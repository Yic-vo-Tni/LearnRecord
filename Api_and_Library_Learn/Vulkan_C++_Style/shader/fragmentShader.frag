#version 450

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform fragUBO{
    vec3 color;
}fragUbo;

void main(){
    outColor = vec4(fragUbo.color, 1.f);
}
