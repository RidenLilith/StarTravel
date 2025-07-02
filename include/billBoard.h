#ifndef BILLBOARD_CLASS_H
#define BILLBOARD_CLASS_H

#include <glm/glm.hpp>
#include "ShaderClass.h"
#include "texture.h"

class Billboard {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    float emissiveStrength = 0.0f;

    Texture* texture;
    Shader* shader;
    GLuint VaoID;

    Billboard(glm::vec3 pos, glm::vec3 scl, Texture* tex, Shader* shdr, GLuint vao);

    void Translate(glm::vec3 offset);
    void Rotate(float angle, glm::vec3 axis);
    void Scale(glm::vec3 scaleFactor);
    void Render();
};

#endif
