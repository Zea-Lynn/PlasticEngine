#pragma once
#include "defines.h"
#include "includes.h"

// Imediat Mode Graphical User Interface.
struct UI {
        enum class key_action : u8{
                up,
                down,
                left,
                right,
                insert_mode,
                activate,
                escape,
                focus_gui,
                //Count value not an action.
                key_action_count,
        }; 
        key current_key_pressed = key::none;
        key key_mapping[(size_t)key_action::key_action_count];
        bool gui_is_focused;
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

        static inline auto generate_font_atlas(char const * font_path, size_t line_height) noexcept -> std::optional<Font_Atlas>{
                Font_Atlas atlas;
                atlas.line_height = line_height;
                auto const font = load_ttf_font(font_path);
                stbtt_fontinfo * info = &atlas.font_info;
                if(not stbtt_InitFont(info, font.data(), 0)) return std::nullopt;

                f32 scale = stbtt_ScaleForPixelHeight(info, line_height);
                s32 ascent, descent, line_gap;
                stbtt_GetFontVMetrics(info, &ascent, &descent, &line_gap);

                atlas.extent.width = line_height * 50; 
                atlas.extent.height = line_height;
                //TODO pass in allocator.
                atlas.bitmap = (u8*)malloc(atlas.extent.width * atlas.extent.height);
                stbtt_pack_context pc;
                stbtt_PackBegin(&pc, atlas.bitmap, atlas.extent.width, atlas.extent.height, 0, 1, nullptr);
                stbtt_PackFontRange(&pc, font.data(), 0, (f32)line_height, 0, INT8_MAX, atlas.packed_chars);
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
                stbi_write_png("test.png", atlas.extent.width, atlas.extent.height, STBI_grey, atlas.bitmap, 0);
                return atlas;
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
                }type;
        } elements[max_element_count] = {"", {}, Element::Type::none, 0};
        //This gets remade every frame.
        u8 elements_to_draw_count = 0;
        s8 elements_to_draw[max_element_count] = {-1};

        //Also begins
        static inline constexpr auto initalize() noexcept -> UI {
                auto gui = UI{};
                gui.key_mapping[(size_t)key_action::left] = key::h;
                gui.key_mapping[(size_t)key_action::down] = key::j;
                gui.key_mapping[(size_t)key_action::up] = key::k;
                gui.key_mapping[(size_t)key_action::right] = key::l;
                gui.key_mapping[(size_t)key_action::insert_mode] = key::i;
                gui.key_mapping[(size_t)key_action::activate] = key::enter;
                gui.key_mapping[(size_t)key_action::escape] = key::esc;
                gui.key_mapping[(size_t)key_action::focus_gui] = key::f1;
                return gui;
        }

        inline constexpr auto begin(u32 screen_width, u32 screen_height, f32 mouse_x, f32 mouse_y, key current_key_pressed, Font_Atlas * atlas) noexcept{
                this->screen_width = screen_width;  
                this->screen_height = screen_height;
                this->atlas = atlas;
                if(not gui_is_focused) return;
                //gui.key_mapping[(size_t)key_action::left] = key::h;
                if(key_mapping[(size_t)key_action::down] == current_key_pressed and not just_moved_cursor){
                        s8 closest_element_index = -1;
                        for(auto i = 0; i < current_element_count; ++i){
                                if(i == element_under_cursor) continue;
                                if(elements[element_under_cursor].position.row < elements[i].position.row){
                                        if(elements[element_under_cursor].position.distance(elements[i].position) < elements[element_under_cursor].position.distance(elements[closest_element_index].position)){
                                                closest_element_index = i;
                                        }
                                }
                        }
                        if(closest_element_index >= 0){
                                element_under_cursor = closest_element_index;
                                just_moved_cursor = true;
                        }
                }
                //gui.key_mapping[(size_t)key_action::up] = key::k;
                //gui.key_mapping[(size_t)key_action::right] = key::l;
        }

        struct Rect{
                f32 x = -1,y = -1,w = 0,h = 0,tu = 0,tv = 0,tw = 1,th = 1;
                Color color;
        };

        struct Mesh{
                std::vector<glm::vec2> positions; 
                std::vector<Texuv> texuvs;
                std::vector<Color> colors;
                std::vector<u32> indices;
                constexpr auto add_rect(Rect rect) {
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
                }
        };

        constexpr auto reset_elements_to_draw() noexcept{
                std::fill(elements_to_draw, elements_to_draw + max_element_count, -1);
                elements_to_draw_count = 0;
        }

        constexpr auto finish_and_draw() noexcept {
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

                u32 current_index = 0;

                for(auto i = 0; i < elements_to_draw_count; ++i){
                        auto const & element = elements[elements_to_draw[i]];
                        if(element.type == Element::Type::button){
                                //TODO: dont use a c string.
                                auto char_count = strlen(element.name);

                                auto button_y = -1 + (f32)(atlas->line_height * i)/screen_height;
                                auto current_char_offset = 0;

                                for(auto c = 0; c < char_count; ++c){
                                        auto stb_data = atlas->packed_chars[element.name[c]];
                                        auto char_y_offset = stb_data.yoff2/screen_height;
                                        auto atlas_offset = atlas->offsets[element.name[c]];
                                        auto char_height = (f32)atlas_offset.h/screen_height;
                                        mesh.add_rect({
                                                .x = (f32)current_char_offset/screen_width - 1,
                                                .y = button_y + char_y_offset + (((f32)atlas->line_height/screen_height) - char_height),
                                                .w = (f32)atlas_offset.w/screen_width,
                                                .h = char_height,
                                                .tu = (f32)atlas_offset.x/atlas->extent.width,
                                                .tv = (f32)atlas_offset.y/atlas->extent.height,
                                                .tw = (f32)atlas_offset.w/atlas->extent.width,
                                                .th = (f32)atlas_offset.h/atlas->extent.height,
                                                .color = {1,1,1,1},
                                        });
                                        current_char_offset += stb_data.xadvance;
                                }
                                //Background.
                                // mesh.add_rect({
                                //         .x = -1, 
                                //         .y= button_y, 
                                //         .w=button_width, 
                                //         .h = screen_space_character_height, 
                                //         .color = {0,1,0,1}
                                // });
                        }
                }

                reset_elements_to_draw();
                return mesh;
        }


        inline auto add_element_to_draw(s8 element_id) noexcept{
                elements_to_draw[elements_to_draw_count] = element_id;
                ++elements_to_draw_count;
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
                if(*button_id == element_under_cursor){
                        if(current_key_pressed == key_mapping[(size_t)key_action::activate]) element_is_active = true;
                        else element_is_active = false;
                }
                add_element_to_draw(*button_id);
                return *button_id == element_under_cursor and element_is_active;
        }
};
