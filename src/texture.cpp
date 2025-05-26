#include "texture.h"
#include "stb/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& filePath, GLenum textureUnit)
    : unit(textureUnit)
{
    glGenTextures(1, &ID);
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Filtros e wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Carregar imagem
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Falha ao carregar textura: " << filePath << "\n";
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}