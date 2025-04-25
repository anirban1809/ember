#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture1;  // The diffuse texture
uniform vec3 diffuseColor;   // Material diffuse color
uniform vec3 viewPos;

const int MAX_LIGHTS = 256;

uniform int lightCount;

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[MAX_LIGHTS];

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    vec3 specularColor = vec3(1.0);

    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * diffuseColor * lights[i].color;

        // Specular component
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        vec3 specular = spec * specularColor * lights[i].color;

        result += diffuse;
    }

    FragColor = vec4(result, 1.0);
}