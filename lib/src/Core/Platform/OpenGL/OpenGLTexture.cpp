#include "Core/Platform/OpenGL/OpenGLTexture.h"
#include "Core/Texture.h"
#include <iostream>
#include <stb/stb_image.h>
#include <GL/glew.h>

void OpenGLTexture::LoadFromFile(const std::string& path, bool srgb) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data =
        stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    GLenum internalFormat = GL_RGB;
    GLenum format = GL_RGB;

    if (channels == 4) {
        internalFormat = srgb ? GL_SRGB_ALPHA : GL_RGBA;
        format = GL_RGBA;
    }

    if (channels == 3) {
        internalFormat = srgb ? GL_SRGB : GL_RGB;
        format = GL_RGB;
    }

    if (channels == 1) {
        internalFormat = format = GL_RED;
    }

    glGenTextures(1, &rendererId);
    glBindTexture(GL_TEXTURE_2D, rendererId);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    std::cout << "Loaded texture: " << path << std::endl;
    std::cout << "Color format" << internalFormat << std::endl;
}

void OpenGLTexture::Bind(uint32 slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, rendererId);
}

void OpenGLTexture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

OpenGLTexture::OpenGLTexture(const std::string& path, TextureType type,
                             bool srgb)
    : path(path), type(type) {
    LoadFromFile(path, srgb);
}

uint32 OpenGLTexture::GetID() const { return rendererId; }

const std::string& OpenGLTexture::GetPath() const { return path; }

TextureType OpenGLTexture::GetType() const { return type; }

OpenGLTexture::~OpenGLTexture() {}