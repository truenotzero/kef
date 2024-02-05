#version 450 core

layout (location = 0)
in vec4 aPos;

// layout (location=1)
// in vec4 aCol;

uniform float brightness;
uniform vec3 uCol;

out vec4 col;

void main() {
    float d = brightness - 0.5f;
    gl_Position = vec4(aPos.x, aPos.y + d, aPos.zw);
    col = vec4(uCol, 1.0);
}
