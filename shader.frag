#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Sphere{
    float x,y,z,radius;
};

layout(location = 0) in vec4 in_color;
layout(location = 1) in vec2 in_tex_uv;

layout(location = 0) out vec4 out_color;
layout(binding = 1) uniform sampler2D sampler;

void main() {

    out_color = texture(sampler, in_tex_uv);

    // vec3 normal = normalize(in_color.xyz);
    //
    // vec3 light_direction = vec3(0.7, 0.7, 0.7);
    // float shininess = 50.0;
    // vec4 I = vec4(1.0, 1.0, 1.0, 1.0);
    // vec4 Ia = vec4(0.3, 0.3, 0.3, 0.3);
    // vec4 Ks = vec4(1.0, 1.0, 1.0, 1.0);
    // vec4 Kd = vec4(0.0, 1.0, 1.0, 1.0);
    //
    // vec3 vertex = normalize(in_color.xyz);
    // float cos_theta = dot(normal, light_direction);
    // vec3 h = normalize(normal + light_direction);
    // float cos_phi = dot(normal, h);
    //
    // out_color = I * ((max(0, cos_theta) * Kd) + pow(max(0, cos_phi), shininess) * Ks) + Ia * Kd;

}
