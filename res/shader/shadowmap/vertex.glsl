#version 450 core

layout (location = 0)
in vec4 aPos;

uniform mat4 uModel;
uniform mat4 uLight;

void main() {
    gl_Position = uLight * uModel * aPos;
}
