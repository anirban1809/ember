#version 330 core

layout(location = 0) in vec3 aPos;        // Position
layout(location = 1) in vec2 aTexCoord;   // Texture coordinate
layout(location = 2) in vec3 aNormal;     // Normal
layout(location = 3) in vec3 aTangent;    // Tangent
layout(location = 4) in vec3 aBitangent;  // Bitangent

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;  // Fragment position in world space
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Transform the vertex position into world space:
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // Transform the normal, tangent, and bitangent correctly
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalize(normalMatrix * aNormal);
    Tangent = normalize(mat3(model) * aTangent);
    Bitangent = normalize(mat3(model) * aBitangent);

    // Pass texture coordinates
    TexCoord = aTexCoord;

    // Final transformation into clip space:
    gl_Position = projection * view * worldPos;
}
