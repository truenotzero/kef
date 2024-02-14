#version 450 core

layout (location = 0)
in vec4 aPos;

layout (location=2)
in vec4 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 uv;
out vec4 normal;

void main() {
    normal = aNormal;
    gl_Position = uProj * uView * uModel * aPos;
}
