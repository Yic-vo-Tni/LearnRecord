#version 450

//layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec2 position;

//vec3 colors[3] = vec3[](
//    vec3(1.f, 0.f, 0.f),
//    vec3(0.f, 1.f, 0.f),
//    vec3(0.f, 0.f, 1.f)
//);

void main(){
    gl_Position = vec4(position, 0.f, 1.f);
//    fragColor = colors[gl_VertexIndex];
}