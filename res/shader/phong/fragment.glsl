#version 450 core

// in vec2 uv;
in vec3 normal;
in vec3 worldPos;
in vec4 lightSpacePos;
out vec4 fragCol;

//uniform sampler2D tex;
layout (binding=0)
uniform sampler2D uShadowMap;
uniform vec3 uGlobalLightCol;
uniform vec3 uGlobalLightPos;
uniform vec3 uViewPos;

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiny;
};


// pos is the fragment's light-space position
float shadow(vec4 pos, vec3 ld, vec3 n) {
    vec2 poissionDisk[4] = vec2[] (
        vec2( -0.94201624, -0.39906216 ),
        vec2( 0.94558609, -0.76890725 ),
        vec2( -0.094184101, -0.92938870 ),
        vec2( 0.34495938, 0.29387760 )
    );
    // normalize to clip-space [-1,1]
    vec3 p = pos.xyz / pos.w;
    // normalize to [0,1] for use with the depth map
    p = 0.5f * p + 0.5f;

    if (p.z > 1.0f) {
        return 0.0f;
    }

    // the depth map represents the distance between the light
    // and the nearest point to it at a given xy coord
    // i.e depth = distance from light
    // depth of the point nearest the light
    // depth of the current fragment
    float d = p.z;
    float b = 0.0f;
    // b = max(0.05f * (1.0f - dot(n, ld)), 0.005f);

    // float th = clamp(dot(n,ld), 0.0f, 1.0f);
    // b = 0.005f * tan(acos(th));
    // b = clamp(b, 0.0f, 0.01f);

    // b = 0.005f;

    float ret = 0.0f;
    for (int i = 0; i < 4; ++i) {
        if (texture(uShadowMap, p.xy + poissionDisk[i] / 1000.0f).r < d - b) {
            ret += 0.25f;
        }
    }
    return ret;

    // float c = texture(uShadowMap, p.xy).r;
    // return d - b > c ? 1.0f : 0.0f;
}

vec3 diffuse(vec3 ld, vec3 vd, vec3 n, float ambient_strength, vec3 ambient_color, float diffuse_strength, vec3 diffuse_color) {
    vec3 diff = max(dot(n, ld), 0.0f) * diffuse_strength * diffuse_color;
    vec3 ambi = ambient_strength * ambient_color;

    return ambi + diff;
}

// calculates lighting values based on the phong shading model
// ld is the normalized light direction (from the light to the fragment)
// n is the normalized surface normal
// vd is the normalized view direction (from the camera to the fragment)
// for ambient, diffuse and specular x,y,z are the color and w is the strength of the component
vec3 phong(vec3 ld, vec3 vd, vec3 n, float ambient_strength, vec3 ambient_color, float diffuse_strength, vec3 diffuse_color, float specular_strength, vec3 specular_color, float shininess) {
    vec3 rd = reflect(ld, n);

    float sh = 128.0f * shininess;
    vec3 spec = pow(max(dot(vd, rd), 0.0f), sh) * specular_strength * specular_color;

    return diffuse(ld, vd, n, ambient_strength, ambient_color, diffuse_strength, diffuse_color) + spec;
}

vec3 blinn_phong(vec3 ld, vec3 vd, vec3 n, float ambient_strength, vec3 ambient_color, float diffuse_strength, vec3 diffuse_color, float specular_strength, vec3 specular_color, float shininess) {
    vec3 h = normalize(ld + vd);

    float sh = 128.0f * shininess;
    vec3 spec = pow(max(dot(n, h), 0.0f), sh) * specular_strength * specular_color;

    return diffuse(ld, vd, n, ambient_strength, ambient_color, diffuse_strength, diffuse_color) + spec;
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

    float s = 1.0f - shadow(lightSpacePos, ld, n);
    vec3 p = blinn_phong(ld, vd, n, 0.05f, uGlobalLightCol, s, uGlobalLightCol, s, uGlobalLightCol, 0.75f);

    // vec3 light = (ambient + diffuse + specular) * uGlobalLightCol * objCol;
    vec3 light = p * objCol;
    // light = attenuate(light, light_direction, 1.0f, 0.0014f, 0.000007f); // 3250 range
    // light = attenuate(light, light_direction, 1.0f, 0.027f, 0.0028f); // 160 range
    light = attenuate(light, light_direction, 1.0f, 0.07f, 0.0017f); // 65 range
    // light = attenuate(light, light_direction, 1.0f, 0.14f, 0.07f); // 32 range
    fragCol = vec4(light, 1.0f);
}
