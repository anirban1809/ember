#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform sampler2D u_albedoMap;     // Albedo texture
uniform sampler2D u_normalMap;     // Normal map
uniform sampler2D u_roughnessMap;  // roughness map
uniform sampler2D u_HeightMap;     // heightmap

uniform vec3 u_AlbedoColor;  // Material color if no texture
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_HeightScale;
uniform vec3 viewPos;

const int MAX_LIGHTS = 256;

uniform int lightCount;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[MAX_LIGHTS];

vec2 ParallaxOcclusionMapping(vec2 texCoords, vec3 viewDirTS,
                              float heightScale) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;

    float numLayers =
        mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDirTS)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    vec2 P = viewDirTS.xy * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(u_HeightMap, currentTexCoords).r;

    // Previous values to interpolate later
    vec2 prevTexCoords = currentTexCoords;
    float prevDepthMapValue = currentDepthMapValue;

    // Step forward until we go past surface
    while (currentLayerDepth < currentDepthMapValue) {
        prevTexCoords = currentTexCoords;
        prevDepthMapValue = currentDepthMapValue;

        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(u_HeightMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    // Binary search refinement between previous and current TexCoords
    const int refinementSteps = 5;
    for (int i = 0; i < refinementSteps; ++i) {
        vec2 midTexCoords = (currentTexCoords + prevTexCoords) * 0.5;
        float midDepthMapValue = texture(u_HeightMap, midTexCoords).r;
        float midLayerDepth = (currentLayerDepth - layerDepth * 0.5);

        if (midLayerDepth < midDepthMapValue) {
            prevTexCoords = midTexCoords;
            prevDepthMapValue = midDepthMapValue;
            currentLayerDepth = midLayerDepth;
        } else {
            currentTexCoords = midTexCoords;
            currentDepthMapValue = midDepthMapValue;
        }
    }

    return currentTexCoords;
}

void main() {
    vec3 viewDir = normalize(viewPos - FragPos);
    mat3 TBN =
        mat3(normalize(Tangent), normalize(Bitangent), normalize(Normal));

    vec3 viewDirTS = normalize(TBN * viewDir);
    float height = texture(u_HeightMap, TexCoord).r;
    vec2 displacedTexCoord =
        ParallaxOcclusionMapping(TexCoord, viewDirTS, u_HeightScale);

    displacedTexCoord = clamp(displacedTexCoord, 0.0, 1.0);

    // Sample and remap normal map
    vec3 sampledNormal = texture(u_normalMap, displacedTexCoord).xyz;
    sampledNormal = sampledNormal * 2.0 - 1.0;  // Remap [0,1] -> [-1,1]

    // transform normal into world space

    vec3 worldNormal = normalize(TBN * sampledNormal);

    // Sample albedo texture (color texture)
    vec3 albedo = texture(u_albedoMap, displacedTexCoord).rgb;
    if (length(albedo) < 0.001) {
        albedo = u_AlbedoColor;
    }

    // 4. Sample roughness texture
    float roughness = texture(u_roughnessMap, displacedTexCoord).r;
    if (roughness == 0.0) {
        roughness = u_Roughness;
    }

    // 4. Lighting
    vec3 result = vec3(0.0);
    vec3 specularColor = vec3(1.0);  // Simple white specular highlights

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);

        // Diffuse
        float diff = max(dot(worldNormal, lightDir), 0.0);
        vec3 diffuse = diff * albedo * lights[i].color;

        // // Specular
        vec3 reflectDir = reflect(-lightDir, worldNormal);
        // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);

        float shininess = mix(4.0, 256.0, 1.0 - roughness);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * specularColor * lights[i].color;

        result += diffuse + specular;
    }

    FragColor = vec4(result, 1.0);
}
