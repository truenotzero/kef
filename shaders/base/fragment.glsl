#version 450 core

in vec2 uv;
in vec4 normal;
out vec4 fragCol;

uniform sampler2D tex;

void main() {
    fragCol = texture(tex, uv);
}
