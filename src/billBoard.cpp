#include "Billboard.h"
#include <glm/gtc/matrix_transform.hpp>

Billboard::Billboard(glm::vec3 pos, glm::vec3 scl, Texture* tex, Shader* shdr)
    : position(pos), scale(scl), texture(tex), shader(shdr)
{
    float quadVertices[] = {
        // posições        // tex coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };

    GLuint quadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    vao.Bind();
    vbo = VBO(quadVertices, sizeof(quadVertices));
    ebo = EBO(quadIndices, sizeof(quadIndices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.Unbind();
}

void Billboard::Render(const glm::mat4& view, const glm::mat4& projection) {
    shader->Activate();
    texture->Bind();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    shader->SetMat4("model", model);
    shader->SetMat4("view", view);
    shader->SetMat4("projection", projection);

    vao.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

void Billboard::Delete() {
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
}
