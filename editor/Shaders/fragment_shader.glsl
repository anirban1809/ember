#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

// Samplers
uniform sampler2D u_albedoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_roughnessMap;
uniform sampler2D u_HeightMap;

// Texture presence flags
uniform bool u_HasAlbedoMap;
uniform bool u_HasNormalMap;
uniform bool u_HasRoughnessMap;
uniform bool u_HasHeightMap;

// Material constants
uniform vec3 u_AlbedoColor;
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

    vec2 prevTexCoords = currentTexCoords;
    float prevDepthMapValue = currentDepthMapValue;

    while (currentLayerDepth < currentDepthMapValue) {
        prevTexCoords = currentTexCoords;
        prevDepthMapValue = currentDepthMapValue;

        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(u_HeightMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

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

    vec2 displacedTexCoord = TexCoord;
    if (u_HasHeightMap)
        displacedTexCoord =
            ParallaxOcclusionMapping(TexCoord, viewDirTS, u_HeightScale);

    displacedTexCoord = clamp(displacedTexCoord, 0.0, 1.0);

    // Albedo
    vec3 albedo = u_AlbedoColor;
    if (u_HasAlbedoMap) {
        albedo = texture(u_albedoMap, displacedTexCoord).rgb;
    } else {
        albedo = u_AlbedoColor;
    }

    // Normal
    vec3 sampledNormal = vec3(0.0, 0.0, 1.0);
    if (u_HasNormalMap) {
        sampledNormal = texture(u_normalMap, displacedTexCoord).xyz;
        sampledNormal = sampledNormal * 2.0 - 1.0;
    }
    vec3 worldNormal = normalize(TBN * sampledNormal);

    // Roughness
    float roughness = u_Roughness;
    if (u_HasRoughnessMap) {
        roughness = texture(u_roughnessMap, displacedTexCoord).r;
    } else {
        roughness = u_Roughness;
    }

    // Lighting
    vec3 result = vec3(0.0);
    vec3 specularColor = vec3(1.0);

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);

        float diff = max(dot(worldNormal, lightDir), 0.0);
        vec3 diffuse = diff * albedo * lights[i].color;

        vec3 reflectDir = reflect(-lightDir, worldNormal);
        float shininess = mix(4.0, 256.0, 1.0 - roughness);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * specularColor * lights[i].color;

        result += diffuse + specular;
    }

    FragColor = vec4(result, 1.0);
}
