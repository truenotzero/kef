#version 450 core

layout (location = 0)
in vec4 aPos;

layout (location=1)
in vec2 aUV;

layout (location=2)
in vec4 aNormal;

out vec2 uv;
out vec4 normal;

void main() {
    uv = aUV;
    normal = aNormal;
    gl_Position = aPos;
}
