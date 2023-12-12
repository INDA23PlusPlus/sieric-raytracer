#version 460

layout(location = 0) in vec2 pos;
out vec2 npos;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    npos = pos;
}
