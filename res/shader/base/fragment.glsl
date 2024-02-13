#version 450 core

// in vec2 uv;
in vec4 normal;
out vec4 fragCol;

uniform vec3 uGlobalLight;

void main() {
    fragCol = vec4(uGlobalLight, 1.0f);
}
