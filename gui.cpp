#pragma once
#include "defines.h"
#include "includes.h"

// Imediat Mode Graphical User Interface.
struct UI {
        enum class Action : u8{
                none,
                up,
                down,
                left,
                right,
                insert_mode,
                activate,
                escape,
                focus_gui,
                //Count value not an action.
                ui_action_count,
        }; 

        Key current_key_pressed = Key::none;
        Key key_mapping[(size_t)Action::ui_action_count];
        bool ui_is_focused;
        u8 element_under_cursor;
        bool just_moved_cursor;
        bool element_is_active;
        u32 screen_width, screen_height;
        u32 max_buffer_size;
        //As fraction of sceenspace
        f32 font_width = 0, font_height = 0;

        //TODO support more than halfwidth ascii.
        struct Font_Atlas{
                struct extent{ u16 width = 0, height = 0; } extent;
                u8 line_height = 0;
                struct Offset{ u16 x = 0, y = 0, w = 0, h = 0; } offsets[INT8_MAX];
                u8 * bitmap = nullptr;
                stbtt_fontinfo font_info;
                stbtt_packedchar packed_chars[UINT8_MAX] = {0};
        } * atlas;

        static inline auto load_ttf_font(char const * path){
                auto file = std::ifstream(path, std::ios::ate | std::ios::binary);
                if(not file){
                        puts("file does not exist");
                        exit(420);
                }
                auto const fileSize = (size_t)file.tellg();
                auto buffer = std::vector<u8>(fileSize);
                file.seekg(0);
                //Why does this have to be char.
                file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
                std::puts(std::format("loading font {} with bytecount {}", path, buffer.size()).c_str());
                return buffer;
        }

        static inline auto generate_font_atlas(u8 const * data, size_t size, size_t line_height)noexcept -> std::optional<Font_Atlas>{
                Font_Atlas atlas;
                atlas.line_height = line_height;
                stbtt_fontinfo * info = &atlas.font_info;
                //TODO: fix this
                if(not stbtt_InitFont(info, data, 0)) return std::nullopt;
                f32 font_size = stbtt_ScaleForPixelHeight(info, line_height);

                atlas.extent.width = line_height * 50; 
                atlas.extent.height = line_height;
                //TODO pass in allocator.
                atlas.bitmap = (u8*)malloc(atlas.extent.width * atlas.extent.height);
                stbtt_pack_context pc;
                stbtt_PackBegin(&pc, atlas.bitmap, atlas.extent.width, atlas.extent.height, 0, 1, nullptr);
                stbtt_PackFontRange(&pc, data, 0, line_height, 0, INT8_MAX, atlas.packed_chars);
                stbtt_PackEnd(&pc);
                for(auto i = 0; i < INT8_MAX; ++i){
                        auto packed_stuff = atlas.packed_chars[i];
                        atlas.offsets[i]  = {
                                .x = packed_stuff.x0,
                                .y = packed_stuff.y0,
                                .w = (u16)(packed_stuff.x1 - packed_stuff.x0),
                                .h = (u16)(packed_stuff.y1 - packed_stuff.y0),
                        };
                }
                stbi_write_png("font.png", atlas.extent.width, atlas.extent.height, STBI_grey, atlas.bitmap, 0);
                return atlas;
        }

        static inline auto generate_font_atlas_from_file(char const * font_path, size_t line_height) noexcept -> std::optional<Font_Atlas>{
                auto const font = load_ttf_font(font_path);
                return generate_font_atlas(font.data(), font.size(), line_height);
        }

        static inline auto genearte_default_font_atlas(size_t line_height){
                return generate_font_atlas(NotoSans_Regular_ttf, NotoSans_Regular_ttf_len, line_height);
        }

        //This is increment for every element rendered.
        u8 current_element_count = 0;
        static constexpr auto max_element_count = 10;
        struct Element{
                char const * name;
                struct element_position{
                        u8 col = 0, row = 0, width = 1, height = 1;
                        inline constexpr auto distance(element_position pos) const noexcept{
                                return glm::distance(glm::vec2{pos.col, col}, glm::vec2{pos.row, row});
                        }
                }position;
                enum class Type: u8{
                        none,
                        button,
                        field_f32,
                        field_s64,
                }type;
                u8 attribute_index = 0;
        } elements[max_element_count] = {"", {}, Element::Type::none, 0};
        u8 filed_f32_attribute_count = 0;
        u8 filed_s64_attribute_count = 0;
        f32 field_f32_attributes[max_element_count];
        s64 field_s64_attributes[max_element_count];
        //This gets remade every frame.
        u8 elements_to_draw_count = 0;
        s8 elements_to_draw[max_element_count] = {-1};

        //Also begins
        static inline constexpr auto initalize() noexcept -> UI {
                auto gui = UI{};
                gui.key_mapping[(size_t)Action::left] = Key::h;
                gui.key_mapping[(size_t)Action::down] = Key::j;
                gui.key_mapping[(size_t)Action::up] = Key::k;
                gui.key_mapping[(size_t)Action::right] = Key::l;
                gui.key_mapping[(size_t)Action::insert_mode] = Key::i;
                gui.key_mapping[(size_t)Action::activate] = Key::enter;
                gui.key_mapping[(size_t)Action::escape] = Key::esc;
                gui.key_mapping[(size_t)Action::focus_gui] = Key::f1;
                return gui;
        }

        struct Rect{
                f32 x = -1,y = -1,w = 0,h = 0,tu = 0,tv = 0,tw = 1,th = 1;
                Color color;
        };

        //TODO: this should just become a buffer character codes
        //      and the atlas should just be put onto the gpu and indexed through the shader.
        struct Mesh{
                std::vector<glm::vec2> positions; 
                std::vector<Texuv> texuvs;
                std::vector<Color> colors;
                std::vector<u32> indices;
                constexpr auto add_rect(Rect rect) noexcept try{
                        indices.push_back(positions.size()+0);
                        indices.push_back(positions.size()+1);
                        indices.push_back(positions.size()+2);
                        indices.push_back(positions.size()+2);
                        indices.push_back(positions.size()+1);
                        indices.push_back(positions.size()+3);
                        positions.push_back({rect.x,            rect.y});
                        positions.push_back({rect.x+rect.w,     rect.y});
                        positions.push_back({rect.x,            rect.y+rect.h});
                        positions.push_back({rect.x+rect.w,     rect.y+rect.h});
                        for(auto i = 0; i < 4; ++i) colors.push_back(rect.color);
                        texuvs.push_back({rect.tu,              rect.tv});
                        texuvs.push_back({rect.tu + rect.tw,    rect.tv});
                        texuvs.push_back({rect.tu,              rect.tv + rect.th});
                        texuvs.push_back({rect.tu + rect.tw,    rect.tv + rect.th});
                }catch(void * e){
                        positions.clear();
                        texuvs.clear();
                        colors.clear();
                        indices.clear();
                }

                constexpr auto add_line(Font_Atlas * atlas, char const * text, size_t text_length, f32 x_offset, f32 y_offset, u32 screen_width, u32 screen_height, Color color) noexcept{
                        //TODO figure out where the scaling issues are comming from.
                        auto constexpr m = 2.5f;
                        auto button_y = -1 + ((f32)(atlas->line_height * y_offset)/screen_height) *m;
                        auto current_char_offset = 0;

                        for(auto c = 0; c < text_length; ++c){
                                auto stb_data = atlas->packed_chars[text[c]];
                                auto char_y_offset = (stb_data.yoff2/screen_height) * m;
                                auto atlas_offset = atlas->offsets[text[c]];
                                auto char_height = ((f32)atlas_offset.h/screen_height) * m;
                                add_rect({
                                        .x = ((f32)current_char_offset/screen_width - 1),
                                        .y = button_y + char_y_offset + ((((f32)atlas->line_height/screen_height) * m) - char_height),
                                        .w = ((f32)atlas_offset.w/screen_width) * m,
                                        .h = char_height,
                                        .tu = (f32)atlas_offset.x/atlas->extent.width,
                                        .tv = (f32)atlas_offset.y/atlas->extent.height,
                                        .tw = (f32)atlas_offset.w/atlas->extent.width,
                                        .th = (f32)atlas_offset.h/atlas->extent.height,
                                        .color = color,
                                });
                                current_char_offset += stb_data.xadvance * m;
                        }
                }
        };

        constexpr auto reset_elements_to_draw() noexcept{
                std::fill(elements_to_draw, elements_to_draw + max_element_count, -1);
                elements_to_draw_count = 0;
                filed_s64_attribute_count = 0;
                filed_f32_attribute_count = 0;
        }

        constexpr auto finish_and_draw(u32 screen_width, u32 screen_height, Font_Atlas * atlas, Action  action = Action::none) noexcept {
                //TODO: check if anything is actually different before recalculating everything.
                this->screen_width = screen_width;  
                this->screen_height = screen_height;
                this->atlas = atlas;

                if(action == Action::up){
                        for(auto i = 0; i < elements_to_draw_count; ++i){
                                if(element_under_cursor == elements_to_draw[i]){
                                        //Alread at top.
                                        if(i == 0){
                                                element_under_cursor = elements_to_draw[elements_to_draw_count-1];
                                                goto valid_cursor;
                                        } else {
                                                element_under_cursor = elements_to_draw[i-1];
                                                goto valid_cursor;
                                        }
                                }
                        }

                element_under_cursor = elements_to_draw[0];
        valid_cursor:
                }
                if(action == Action::down){
                        for(auto i = 0; i < elements_to_draw_count; ++i){
                                if(element_under_cursor == elements_to_draw[i]){
                                        //Alread at top.
                                        if(i == elements_to_draw_count-1){
                                                element_under_cursor = elements_to_draw[0];
                                                goto valid_cursor2;
                                        } else {
                                                element_under_cursor = elements_to_draw[i+1];
                                                goto valid_cursor2;
                                        }
                                }
                        }

                element_under_cursor = elements_to_draw[0];
        valid_cursor2:
                }

                //TODO store this in preallocated area.
                auto mesh = Mesh{};
                //calculate bounding box size
                auto grid_row_length = 0;

                for(auto i = 0; i < elements_to_draw_count; ++i){
                auto const & element = elements[elements_to_draw[i]];
                        if(element.type == Element::Type::button){
                                auto button_size = strlen(element.name);
                                if(grid_row_length < button_size) grid_row_length = button_size;
                        }
                }

                for(auto i = 0; i < elements_to_draw_count; ++i){
                        auto const & element = elements[elements_to_draw[i]];
                        bool is_hovered = element_under_cursor == elements_to_draw[i];
                        if(element.type == Element::Type::button){
                                //TODO: dont use a c string.
                                auto char_count = strlen(element.name);
                                auto color = Color{1,1,1,1};
                                if(is_hovered) color = Color{1,0,1,1};
                                mesh.add_line(atlas, element.name, char_count, 0, i, screen_width, screen_height, color);
                        }else if(element.type == Element::Type::field_s64){
                                auto constexpr s64_char_size = 20;
                                char buf[s64_char_size] = {'A'};
                                snprintf(buf, s64_char_size, "%010ld", field_s64_attributes[element.attribute_index]);
                                auto color = Color{1,1,1,1};
                                if(is_hovered) color = Color{1,0,1,1};
                                mesh.add_line(atlas, buf, s64_char_size, 0, i, screen_width, screen_height, color);
                        }
                }
                reset_elements_to_draw();
                return mesh;
        }


        inline auto add_element_to_draw(s8 element_id) noexcept{
                elements_to_draw[elements_to_draw_count] = element_id;
                ++elements_to_draw_count;
        }

        inline auto initalize_or_set_element_id(s8 * element_id) noexcept -> bool{
                if(element_id == nullptr) return false;
                if(*element_id < 0) *element_id = current_element_count;
                ++current_element_count;
                return true;
        }

        //TODO: add a way to style what is going on in this value, like how many digits it's allowed to show.
        inline auto field_s64(s8 * field_s64_id, char const * label, s64 value)noexcept -> bool{
                if(not initalize_or_set_element_id(field_s64_id)) return false;
                elements[*field_s64_id] = {
                        .name = label,
                        .position = {},
                        .type=Element::Type::field_s64,
                        .attribute_index = filed_s64_attribute_count
                };
                field_s64_attributes[filed_s64_attribute_count] = value;
                ++filed_s64_attribute_count;
                add_element_to_draw(*field_s64_id);
                return true;
        }

        //if the button id value is less than 0 one will be assigned to the value passed in.
        //if the id is null than it will not render the button.
        inline auto button(s8 * button_id, char const * string) noexcept -> bool{
                if(button_id == nullptr) return false;
                if(*button_id < 0){
                        *button_id = current_element_count;
                        elements[*button_id] = {
                                .name = string,
                                .position={.col=0,.row=1,.width=static_cast<u8>(strlen(string)), .height =1 }, 
                                .type=Element::Type::button, 
                        };
                        ++current_element_count;
                }  
                // if(*button_id == element_under_cursor){
                //         if(current_key_pressed == key_mapping[(size_t)key_action::activate]) element_is_active = true;
                //         else element_is_active = false;
                // }
                add_element_to_draw(*button_id);
                return *button_id == element_under_cursor and element_is_active;
        }
};
