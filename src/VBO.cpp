#include "VBO.h"

VBO::VBO(std::vector<float> vertices, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    // Introduce the vertices into the VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
    glDeleteBuffers(1,&ID);
}