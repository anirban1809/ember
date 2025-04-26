#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform sampler2D u_albedoMap;  // Albedo texture
uniform sampler2D u_normalMap;  // Normal map

uniform vec3 u_AlbedoColor;  // Material color if no texture
uniform float u_Metallic;
uniform float u_Roughness;
uniform vec3 viewPos;

const int MAX_LIGHTS = 256;

uniform int lightCount;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[MAX_LIGHTS];

void main() {
    // 1. Sample and remap normal map
    vec3 sampledNormal = texture(u_normalMap, TexCoord).xyz;
    sampledNormal = sampledNormal * 2.0 - 1.0;  // Remap [0,1] -> [-1,1]

    // 2. Build TBN matrix and transform normal into world space
    mat3 TBN =
        mat3(normalize(Tangent), normalize(Bitangent), normalize(Normal));
    vec3 worldNormal = normalize(TBN * sampledNormal);

    // 3. Sample albedo texture (color texture)
    vec3 albedo = texture(u_albedoMap, TexCoord).rgb;
    if (length(albedo) < 0.001) {
        albedo = u_AlbedoColor;
    }

    // 4. Lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);
    vec3 specularColor = vec3(1.0);  // Simple white specular highlights

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);

        // Diffuse
        float diff = max(dot(worldNormal, lightDir), 0.0);
        vec3 diffuse = diff * albedo * lights[i].color;

        // Specular
        vec3 reflectDir = reflect(-lightDir, worldNormal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        vec3 specular = spec * specularColor * lights[i].color;

        result += diffuse + specular;
    }

    FragColor = vec4(result, 1.0);
}
