#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
class EBO
{
    public:
        GLuint ID;
        EBO(std::vector<GLuint> indices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};

#endif