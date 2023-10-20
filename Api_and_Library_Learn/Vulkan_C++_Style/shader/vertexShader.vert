//File: vertex_shader.vert

#version 450

layout(location = 0) in vec2 position;

layout(set = 0, binding = 0) uniform vertUBO{
    mat4 project;
    mat4 view;
    mat4 model;
}vertUbo;

void main(){
    gl_Position = vertUbo.project * vertUbo.view * vertUbo.model * vec4(position, 0.f, 0.f);
}
