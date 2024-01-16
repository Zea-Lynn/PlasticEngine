#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Sphere{
    float x,y,z,radius;
};

layout(location = 0) in vec4 in_color;
layout(location = 0) out vec4 out_color;


void main() {
    out_color = vec4(gl_FragCoord.x/1024, gl_FragCoord.y/1024, 0, 0);
}
