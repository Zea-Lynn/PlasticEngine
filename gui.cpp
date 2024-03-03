#pragma once
#include "defines.h"
#include "includes.h"

// Imediat Mode Graphical User Interface.
struct GUI {
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
        f32 screen_x, screen_y, width, height;
        //each grid cell is the size of a fullsize character
        struct grid{
                u8 columns, rows;
                float x, y, width, height;
        } grid;

        //This is increment for every element rendered.
        u8 current_element_count = 0;
        static constexpr auto max_element_count = 10;
        struct element{
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

        } elements[max_element_count] = {"", {}, element::type::none, 0};
        //This gets remade every frame.
        u8 elements_to_render_count = 0;
        s8 elements_to_render[max_element_count] = {-1};

        struct style{
                glm::vec4 resting_color, hot_color, active_color;
        } styles[max_element_count];

        //Also begins
        static inline constexpr auto initalize() noexcept -> GUI {
                auto gui = GUI{};
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

        inline constexpr auto begin(f32 screen_x, f32 screen_y, f32 mouse_x, f32 mouse_y, key current_key_pressed) noexcept{
                grid.x = 0;
                grid.y = 0;
                grid.height = 1;
                grid.width = .5;
                grid.rows = UINT8_MAX;
                grid.columns = 100;
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

        //TODO: return uv and verts to render gui.
        inline constexpr auto end() noexcept {
                struct{
                        std::vector<glm::vec2> vertices; 
                        std::vector<u32> indices;
                } stuff;

                stuff.vertices.clear();
                //draw grid.
                stuff.vertices.push_back({grid.x, grid.y});
                stuff.vertices.push_back({grid.x+grid.width, grid.y});
                stuff.vertices.push_back({grid.x, grid.y+grid.height});
                stuff.vertices.push_back({grid.x+grid.width, grid.y+grid.height});
                stuff.indices.push_back(0);
                stuff.indices.push_back(1);
                stuff.indices.push_back(2);
                stuff.indices.push_back(2);
                stuff.indices.push_back(1);
                stuff.indices.push_back(3);
                auto grid_col_size = (grid.width / (f32)grid.columns);
                auto grid_row_size = (grid.height / (f32)grid.rows);
                size_t current_index = 4;
                //draw elemens on grid.
                for(auto i =0; i < elements_to_render_count and elements_to_render[i] >= 0; ++i){
                        s8 id = elements_to_render[i];
                        f32 element_x = grid.x + (elements[id].position.col * grid_col_size);
                        f32 element_y = grid.y + (elements[id].position.row * grid_row_size);
                        f32 element_width = elements[id].position.width * grid_col_size; 
                        f32 element_height = elements[id].position.height * grid_row_size;
                        stuff.vertices.push_back({element_x, element_y});
                        stuff.vertices.push_back({element_x + element_width, element_y});
                        stuff.vertices.push_back({element_x, element_y + element_height});
                        stuff.vertices.push_back({element_x + element_width, element_y + element_height});
                        stuff.indices.push_back(current_index + 0);
                        stuff.indices.push_back(current_index + 1);
                        stuff.indices.push_back(current_index + 2);
                        stuff.indices.push_back(current_index + 2);
                        stuff.indices.push_back(current_index + 1);
                        stuff.indices.push_back(current_index + 3);
                        current_index +=4;
                        
                        for(u8 char_i = 0;elements[id].name[char_i] != '\0'; ++char_i){
                                //TODO: lookup texture coords for the char in the element.
                                f32 glyph_x = element_x + grid_col_size;
                                f32 glyph_y = element_y;
                                stuff.vertices.push_back({glyph_x, glyph_y});
                                stuff.vertices.push_back({glyph_x + grid_col_size, glyph_y});
                                stuff.vertices.push_back({glyph_x, glyph_y + grid_row_size});
                                stuff.vertices.push_back({glyph_x + grid_col_size, glyph_y + grid_row_size});
                                stuff.indices.push_back(current_index + 0);
                                stuff.indices.push_back(current_index + 1);
                                stuff.indices.push_back(current_index + 2);
                                stuff.indices.push_back(current_index + 2);
                                stuff.indices.push_back(current_index + 1);
                                stuff.indices.push_back(current_index + 3);
                                current_index +=4;
                        }
                }

                //Reset.
                std::fill(elements_to_render, elements_to_render + max_element_count, -1);

                return stuff;
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
                                .type=element::type::button, 
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
