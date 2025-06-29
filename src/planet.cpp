#include "planet.h"
#include <cmath>
#include "stb/stb_image.h"
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265359f

Planet::Planet(int stacks, int sectors, Texture* tex, Shader* sha, GLuint ID)
{
    texture = tex;
    shader = sha;
    float radius = 1.0f;
    VaoID = ID;

    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = PI / 2 - i * PI / stacks; // de +pi/2 a -pi/2
        float xy = radius * cosf(stackAngle);        // raio do círculo atual
        float z = radius * sinf(stackAngle);         // altura

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * 2 * PI / sectors; // de 0 a 2pi

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float u = (float)j / sectors;
            float v = (float)i / stacks;

            // Posição
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // UV
            vertices.push_back(u);
            vertices.push_back(v);

            // Normal (normalizada)
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

        }
    }

    // Criar índices
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            int first = i * (sectors + 1) + j;
            int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void Planet::Translate(glm::vec3 offset)
{
    model = glm::translate(model, offset);
}
void Planet::Rotate(float angle, glm::vec3 axis)
{
    model = glm::rotate(model, glm::radians(angle), axis);
}
void Planet::Scale(glm::vec3 scaleFactor)
{
    model = glm::scale(model, scaleFactor);
}

void Planet::Render()
{
    shader->Activate();
    glBindVertexArray(VaoID);

    texture->Bind();

    GLuint modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}