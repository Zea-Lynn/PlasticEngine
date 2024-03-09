#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_texuv;
layout(location = 2) in vec4 in_color;

layout(location = 0) out vec4 frag_color; 
layout(location = 1) out vec2 frag_tex_uv;

void main() {
    gl_Position = vec4(in_pos, 0, 1.0);
    frag_tex_uv = in_texuv;
    frag_color = vec4(in_color);
}
