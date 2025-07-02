#include "billBoard.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Billboard::Billboard(glm::vec3 pos, glm::vec3 scl, Texture* tex, Shader* shdr, GLuint vaoID)
{
    position = pos;
    scale = scl;
    texture = tex;
    shader = shdr;
    VaoID = vaoID;

    // Model matrix inicial
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
}

void Billboard::Translate(glm::vec3 offset)
{
    position = offset;
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * rotation;
    model = glm::scale(model, scale);
}

void Billboard::Rotate(float angle, glm::vec3 axis)
{
    rotation = glm::rotate(rotation, glm::radians(angle), axis);
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * rotation;
    model = glm::scale(model, scale);
}

void Billboard::Scale(glm::vec3 scl)
{
    scale = scl;
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * rotation;
    model = glm::scale(model, scale);
}

void Billboard::Render()
{
    shader->Activate();
    glBindVertexArray(VaoID);

    texture->Bind();

    GLuint modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
