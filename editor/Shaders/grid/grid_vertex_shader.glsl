#version 330 core

layout(location = 0) in vec2 aPosition;

out vec2 vUV;

void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);

    // Map from [-1, 1] NDC to [0, 1] UV
    vUV = aPosition * 0.5 + 0.5;
}
