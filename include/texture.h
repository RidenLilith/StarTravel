#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
    GLuint ID;

    Texture(const std::string& filePath, GLenum textureUnit = GL_TEXTURE0);

    void Bind() const;
    void Unbind() const;

private:
    GLenum unit;
};

#endif