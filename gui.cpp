#pragma once
#include "defines.h"
#include "includes.h"

// Imediat Mode Graphical User Interface.
struct GUI {
        bool gui_is_focused;
        u8 element_under_cursor;
        bool element_is_active;
        f32 screen_x, screen_y, width, height;
        //each grid cell is the size of a fullsize character
        struct grid{
                u8 columns, rows;
                float x, y, width, height;
        } grid;

        struct element{
                enum class type{
                        none,
                        button,
                        field_f32,
                }type;
                struct element_position{
                        u8 col = 0, row = 0, width = 1, height = 1;
                };
                u8 style_index;
        } elements[UINT8_MAX] = {element::type::none, 0};

        struct element_style{
                glm::vec4 resting_color, hot_color, active_color;
        } styles[UINT8_MAX];



        static inline constexpr auto begin() noexcept -> GUI {
                return GUI{};
        }

        //TODO: return uv and verts to render gui.
        inline constexpr auto end() noexcept {
                
        }

        inline constexpr auto button(u8 const * str, u8 strlen, button_style = {{.5,0,.5,1}, {.7,0,.7,1}, {1,.5,1,1}}) noexcept{
                
        }
};
