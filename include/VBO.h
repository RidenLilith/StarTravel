#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
class VBO
{
    public: 
        GLuint ID;
        VBO(std::vector<float> vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();

};

#endif