#version 450 core

layout (location = 0)
in vec4 aPos;

layout (location=1)
in vec2 aUV;

layout (location=2)
in vec4 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 uv;
out vec4 normal;

void main() {
    uv = aUV;
    normal = aNormal;
    gl_Position = uProjection * uView * uModel * aPos;
}
