#version 450 core

layout (location = 0)
in vec4 aPos;

layout (location=1)
in vec4 aCol;

out vec4 col;

void main() {
    gl_Position = aPos;
    col = aCol;
}
