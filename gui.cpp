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

        //TODO support more than halfwidth.
        struct Font_Atlas{
                struct extent{ u16 width, height; } extent;
                struct char_extent{ u8 width, height;} char_extent;
                struct offset{ u8 x, y; } offsets[INT8_MAX] = {0,0};
        } atlas;

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
                enum class type: u8{
                        none,
                        button,
                        field_f32,
                }type;
                u8 style_index;

        } elements[max_element_count] = {"", {}, Element::type::none, 0};
        //This gets remade every frame.
        u8 elements_to_render_count = 0;
        s8 elements_to_render[max_element_count] = {-1};

        struct style{
                Color resting_color, hot_color, active_color;
        } styles[max_element_count];

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

        inline constexpr auto begin(u32 screen_width, u32 screen_height, f32 mouse_x, f32 mouse_y, key current_key_pressed, Font_Atlas font_atlas) noexcept{
                if(not gui_is_focused) return;
                font_width = (f32)screen_width / font_atlas.extent.width;
                font_height = (f32)screen_height / font_atlas.extent.height;
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
                f32 x,y,w,h,tu = 0,tv = 0,tw = 1,th = 1;
                Color color;
        };

        struct Mesh{
                std::vector<glm::vec2> positions; 
                std::vector<Texuv> texuvs;
                std::vector<Color> colors;
                std::vector<u32> indices;
                constexpr auto add_rect(Rect rect, u32 index) {
                        positions.push_back({rect.x,            rect.y});
                        positions.push_back({rect.x+rect.w,     rect.y});
                        positions.push_back({rect.x,            rect.y+rect.h});
                        positions.push_back({rect.x+rect.w,     rect.y+rect.h});
                        for(auto i = 0; i < 4; ++i) colors.push_back(rect.color);
                        texuvs.push_back({rect.tu,              rect.tv});
                        texuvs.push_back({rect.tu + rect.tw,    rect.tv});
                        texuvs.push_back({rect.tu,              rect.tv + rect.th});
                        texuvs.push_back({rect.tu + rect.tw,    rect.tv + rect.th});
                        indices.push_back(index+0);
                        indices.push_back(index+1);
                        indices.push_back(index+2);
                        indices.push_back(index+2);
                        indices.push_back(index+1);
                        indices.push_back(index+3);
                }
        };


        constexpr auto finish_and_render() noexcept {
                Mesh mesh;
                //draw grid.

                //calculate bounding box size
                auto grid_row_length = 0;

                for(auto i = 0; i < elements_to_render_count; ++i){
                auto const & element = elements[elements_to_render[i]];
                        if(element.type == Element::type::button){
                                auto button_size = strlen(element.name);
                                if(grid_row_length < button_size) grid_row_length = button_size;
                        }
                }

                mesh.add_rect({.x = -1, .y =  -1, .w =  2 * (2.0/5), .h = 2, .color = {1, 0, 1, 1}}, 0);



                // auto grid_col_size = (grid.width / (f32)100);
                // auto grid_row_size = (grid.height / (f32)grid.rows);
                // size_t current_index = 4;
                // //draw elemens on grid.
                // for(auto i =0; i < elements_to_render_count and elements_to_render[i] >= 0; ++i){
                //         s8 id = elements_to_render[i];
                //         f32 element_x = grid.x + (elements[id].position.col * grid_col_size);
                //         f32 element_y = grid.y + (elements[id].position.row * grid_row_size);
                //         f32 element_width = elements[id].position.width * grid_col_size; 
                //         f32 element_height = elements[id].position.height * grid_row_size;
                //         mesh.vertices.push_back({element_x, element_y});
                //         mesh.vertices.push_back({element_x + element_width, element_y});
                //         mesh.vertices.push_back({element_x, element_y + element_height});
                //         mesh.vertices.push_back({element_x + element_width, element_y + element_height});
                //         mesh.indices.push_back(current_index + 0);
                //         mesh.indices.push_back(current_index + 1);
                //         mesh.indices.push_back(current_index + 2);
                //         mesh.indices.push_back(current_index + 2);
                //         mesh.indices.push_back(current_index + 1);
                //         mesh.indices.push_back(current_index + 3);
                //         current_index +=4;
                //         
                //         for(u8 char_i = 0;elements[id].name[char_i] != '\0'; ++char_i){
                //                 //TODO: lookup texture coords for the char in the element.
                //                 f32 glyph_x = element_x + grid_col_size;
                //                 f32 glyph_y = element_y;
                //                 mesh.vertices.push_back({glyph_x, glyph_y});
                //                 mesh.vertices.push_back({glyph_x + grid_col_size, glyph_y});
                //                 mesh.vertices.push_back({glyph_x, glyph_y + grid_row_size});
                //                 mesh.vertices.push_back({glyph_x + grid_col_size, glyph_y + grid_row_size});
                //                 mesh.indices.push_back(current_index + 0);
                //                 mesh.indices.push_back(current_index + 1);
                //                 mesh.indices.push_back(current_index + 2);
                //                 mesh.indices.push_back(current_index + 2);
                //                 mesh.indices.push_back(current_index + 1);
                //                 mesh.indices.push_back(current_index + 3);
                //                 current_index +=4;
                //         }
                // }

                //Reset.
                std::fill(elements_to_render, elements_to_render + max_element_count, -1);

                return mesh;
        }

        inline auto add_element_to_render(s8 element_id) noexcept{
                elements_to_render[elements_to_render_count] = element_id;
                ++elements_to_render_count;
        }

        //if the button id value is less than 0 one will be assigned to the value passed in.
        //if the id is null than it will not render the button.
        inline auto button(s8 * button_id, char const * string, style = {{.5,0,.5,1}, {.7,0,.7,1}, {1,.5,1,1}}) noexcept -> bool{
                if(button_id == nullptr) return false;
                if(*button_id < 0){
                        *button_id = current_element_count;
                        elements[*button_id] = {
                                .name = string,
                                .position={.col=0,.row=1,.width=static_cast<u8>(strlen(string)), .height =1 }, 
                                .type=Element::type::button, 
                        };
                        ++current_element_count;
                }  
                if(*button_id == element_under_cursor){
                        if(current_key_pressed == key_mapping[(size_t)key_action::activate]) element_is_active = true;
                        else element_is_active = false;
                }
                add_element_to_render(*button_id);
                return *button_id == element_under_cursor and element_is_active;
        }
};
