#include "asteroid.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.14159265359f

Asteroid::Asteroid(glm::vec3 startPos, int stacks, int sectors, Texture* tex, Shader* sha, GLuint vaoID)
: Planet(stacks, sectors, tex, sha, vaoID)
{
    position = startPos;
    model = glm::translate(glm::mat4(1.0f), position);

    texture = tex;
    shader = sha;
    VaoID = vaoID;

    float radius = 1.0f;

    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2 - i * PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * PI / sectors;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float u = (float)j / sectors;
            float v = (float)i / stacks;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            vertices.push_back(u);
            vertices.push_back(v);

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
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

void Asteroid::Translate(glm::vec3 offset)
{
    model = glm::translate(model, offset);
    position += offset;
}

void Asteroid::Rotate(float angle, glm::vec3 axis)
{
    rotation = glm::rotate(rotation, glm::radians(angle), axis);
}

void Asteroid::Scale(glm::vec3 scaleFactor)
{
    currentScale = scaleFactor;
    model = glm::scale(model, currentScale);
}

void Asteroid::Render()
{
    shader->Activate();
    glBindVertexArray(VaoID);

    texture->Bind();

    glm::mat4 finalModel = model * rotation;
    GLuint modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModel));

    GLuint emissiveLoc = glGetUniformLocation(shader->ID, "emissiveStrength");
    glUniform1f(emissiveLoc, emissiveStrength);

    GLuint lightColorLoc = glGetUniformLocation(shader->ID, "lightColor");
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
