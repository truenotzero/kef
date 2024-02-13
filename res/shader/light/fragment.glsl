#version 450 core

in vec2 uv;
in vec4 normal;
out vec4 fragCol;

uniform sampler2D tex;
uniform vec3 uGlobalLight;

void main() {
    fragCol = vec4(uGlobalLight, 1.0f);// * texture(tex, uv);
    // fragCol = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
