#version 450 core

// in vec2 uv;
in vec3 normal;
in vec3 worldPos;
out vec4 fragCol;

uniform sampler2D tex;
uniform vec3 uGlobalLightCol;
uniform vec3 uGlobalLightPos;
uniform vec3 uViewPos;

void main() {
    vec3 objCol = vec3(0.6f);

    // light direction
    vec3 ld = normalize(uGlobalLightPos - worldPos);
    // normalized surface normal
    vec3 n = normalize(normal);

    // view direction
    vec3 vd = normalize(uViewPos - worldPos);
    // reflection direction
    vec3 rd = reflect(ld, n);
    
    float ambient = 0.2f;

    float diffuse = max(dot(n, ld), 0.0f);

    float specularStrength = 1.0f;
    float shininess = 128;
    float specular = specularStrength * pow(max(dot(vd, rd), 0.0f), shininess);

    vec3 light = (ambient + diffuse + specular) * uGlobalLightCol * objCol;
    fragCol = vec4(light, 1.0f);
}
