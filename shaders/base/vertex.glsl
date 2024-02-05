#version 450 core

layout (location = 0)
in vec4 aPos;

// layout (location=1)
// in vec4 aCol;

uniform float brightness;
uniform vec3 uCol;

out vec4 col;

void main() {
    gl_Position = aPos;
    col = vec4(brightness * uCol, 1.0);
}
