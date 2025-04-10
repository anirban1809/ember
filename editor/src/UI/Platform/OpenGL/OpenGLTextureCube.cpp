#include "../../../../include/UI/Platform/OpenGL/OpenGLTextureCube.h"
#include <GL/glew.h>
#include <iostream>
#include <stb_image.h>
#include <vector>

OpenGLTextureCube::OpenGLTextureCube(uint32 resolution) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    for (uint32 i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                     resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

std::shared_ptr<TextureCube> OpenGLTextureCube::CreateFromCrossLayout(
    const std::string& path) {
    stbi_set_flip_vertically_on_load(
        false);  // cross layouts are usually upright

    int width, height, channels;
    unsigned char* data =
        stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "[TextureCube] Failed to load image: " << path
                  << std::endl;
        return nullptr;
    }

    if (width % 4 != 0 || height % 3 != 0 || width / 4 != height / 3) {
        std::cerr << "[TextureCube] Image doesn't match 4x3 cubemap cross "
                     "layout (must be 4:3 ratio with square faces)"
                  << std::endl;
        stbi_image_free(data);
        return nullptr;
    }

    int faceSize = width / 4;
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    // Face mapping: {column, row}
    std::pair<int, int> faceOffsets[6] = {
        {2, 1},  // +X
        {0, 1},  // -X
        {1, 0},  // +Y
        {1, 2},  // -Y
        {1, 1},  // +Z
        {3, 1}   // -Z
    };

    for (int face = 0; face < 6; ++face) {
        int ox = faceOffsets[face].first * faceSize;
        int oy = faceOffsets[face].second * faceSize;

        std::vector<unsigned char> faceData(faceSize * faceSize * channels);

        for (int row = 0; row < faceSize; ++row) {
            memcpy(&faceData[row * faceSize * channels],
                   &data[((oy + row) * width + ox) * channels],
                   faceSize * channels);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format, faceSize,
                     faceSize, 0, format, GL_UNSIGNED_BYTE, faceData.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);

    return std::make_shared<OpenGLTextureCube>(
        texID, faceSize);  // adjust to your implementation
}

void OpenGLTextureCube::Bind(uint32 slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void OpenGLTextureCube::BindFaceForWriting(uint32 face) const {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, id, 0);
}
