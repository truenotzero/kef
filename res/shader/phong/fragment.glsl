#version 450 core

// in vec2 uv;
in vec3 normal;
in vec3 worldPos;
out vec4 fragCol;

uniform sampler2D tex;
uniform vec3 uGlobalLightCol;
uniform vec3 uGlobalLightPos;
uniform vec3 uViewPos;

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiny;
};

// calculates lighting values based on the phong shading model
// ld is the normalized light direction (from the light to the fragment)
// n is the normalized surface normal
// vd is the normalized view direction (from the camera to the fragment)
// for ambient, diffuse and specular x,y,z are the color and w is the strength of the component
vec3 phong(vec3 ld, vec3 vd, vec3 n, float ambient_strength, vec3 ambient_color, float diffuse_strength, vec3 diffuse_color, float specular_strength, vec3 specular_color, float shininess) {
    vec3 rd = reflect(ld, n);

    float sh = 128.0f * shininess;
    vec3 spec = pow(max(dot(vd, rd), 0.0f), sh) * specular_strength * specular_color;
    vec3 diff = max(dot(n, ld), 0.0f) * diffuse_strength * diffuse_color;
    vec3 ambi = ambient_strength * ambient_color;

    return ambi + diff + spec;
}

// attenuates light based on distance
// l is the light's color
// d is the direction the light is coming from (not normalized!)
// kC is the constant coefficient (generally 1.0f)
// kL is the is the linear coefficient
// kQ is the quadratic coefficient
vec3 attenuate(vec3 l, vec3 d, float kC, float kL, float kQ) {
    float dist2 = d.x * d.x + d.y * d.y + d.z * d.z;
    float dist = sqrt(dist2);
    float a = 1.0f / (kC + kL * dist + kQ * dist2);
    return a * l;
}

void main() {
    vec3 objCol = vec3(1.0f);

    // light direction
    vec3 light_direction = uGlobalLightPos - worldPos;
    vec3 ld = normalize(light_direction);
    // normalized surface normal
    vec3 n = normalize(normal);

    // view direction
    vec3 vd = normalize(uViewPos - worldPos);

    vec3 p = phong(ld, vd, n, 0.2f, uGlobalLightCol, 1.0f, uGlobalLightCol, 1.0f, uGlobalLightCol, 0.25f);

    // reflection direction
    // vec3 rd = reflect(ld, n);
    
    // float ambient = 0.2f;

    // float diffuse = max(dot(n, ld), 0.0f);

    // float specularStrength = 1.0f;
    // float shininess = 128;
    // float specular = specularStrength * pow(max(dot(vd, rd), 0.0f), shininess);

    // vec3 light = (ambient + diffuse + specular) * uGlobalLightCol * objCol;
    vec3 light = p * objCol;
    // light = attenuate(light, light_direction, 1.0f, 0.0014f, 0.000007f); // 3250 range
    light = attenuate(light, light_direction, 1.0f, 0.027f, 0.0028f); // 160 range
    // light = attenuate(light, light_direction, 1.0f, 0.07f, 0.0017f); // 65 range
    // light = attenuate(light, light_direction, 1.0f, 0.14f, 0.07f); // 32 range
    fragCol = vec4(light, 1.0f);
}
