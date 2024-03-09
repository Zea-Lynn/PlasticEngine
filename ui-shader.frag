#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vert_color;
layout(location = 1) in vec2 in_tex_uv;
layout(binding = 1) uniform sampler2D tex_sampler;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = vert_color * texture(tex_sampler, in_tex_uv);
}
