#version 450 core

layout (location = 0)
in vec4 aPos;

layout (location=1)
in vec2 aUV;

layout (location=2)
in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 uv;
out vec3 normal;
out vec3 worldPos;

void main() {
    uv = aUV;
    normal = aNormal;
    worldPos = (uModel * aPos).xyz;
    gl_Position = uProj * uView * vec4(worldPos, 1.0f);
}
