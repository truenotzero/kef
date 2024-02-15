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
uniform mat4 uLight;

out vec2 uv;
out vec3 normal;
out vec3 worldPos;
out vec4 lightSpacePos;

void main() {
    uv = aUV;
    normal = aNormal;
    worldPos = (uModel * aPos).xyz;
    lightSpacePos = uLight * uModel * aPos;
    gl_Position = uProj * uView * vec4(worldPos, 1.0f);
}
