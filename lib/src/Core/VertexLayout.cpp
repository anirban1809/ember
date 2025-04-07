#include "../../include/Core/VertexLayout.h"

uint32 GetShaderDataTypeSize(ShaderType type) {
    switch (type) {
        case ShaderType::Float:
            return 4;
        case ShaderType::Float2:
            return 4 * 2;
        case ShaderType::Float3:
            return 4 * 3;
        case ShaderType::Float4:
            return 4 * 4;
        case ShaderType::Mat3:
            return 4 * 3 * 3;
        case ShaderType::Mat4:
            return 4 * 4 * 4;
        case ShaderType::Int:
            return 4;
        case ShaderType::Int2:
            return 4 * 2;
        case ShaderType::Int3:
            return 4 * 3;
        case ShaderType::Int4:
            return 4 * 4;
        case ShaderType::Bool:
            return 1;
        default:
            return 0;
    }
}

void VertexLayout::CalculateStride() {
    stride = 0;
    for (const auto& attr : attributes) {
        stride += GetShaderDataTypeSize(attr.type);
    }
}