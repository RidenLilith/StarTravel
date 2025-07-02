#pragma once

#include <glm/glm.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include "texture.h"

class Billboard {
public:
    glm::vec3 position;
    glm::vec3 scale;
    Texture* texture;
    Shader* shader;

    VAO vao;
    VBO vbo;
    EBO ebo;

    Billboard(glm::vec3 pos, glm::vec3 scl, Texture* tex, Shader* shdr);
    void Render(const glm::mat4& view, const glm::mat4& projection);
    void Delete();
};
