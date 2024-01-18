#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec2 inTexUV;

layout(location = 0) out vec4 frag_color; 

void main() {
    gl_Position = vec4(inPosition, 1.0);
    
    mat4 model = ubo.model;
    frag_color = vec4(model[0][0],model[0][1],model[0][2],1);
}
