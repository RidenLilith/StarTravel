#ifndef ASTEROID_H
#define ASTEROID_H

#include <vector>
#include <glm/glm.hpp>
#include "ShaderClass.h"
#include "Texture.h"
#include "planet.h" 

class Asteroid : public Planet {
public:
    Asteroid(glm::vec3 startPos, int stacks, int sectors, Texture* tex, Shader* sha, GLuint vaoID);

    void Translate(glm::vec3 offset);
    void Rotate(float angle, glm::vec3 axis);
    void Scale(glm::vec3 scaleFactor);
    void Render();

    glm::vec3 position;
    float emissiveStrength = 0.0f;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

private:
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 currentScale = glm::vec3(1.0f);

    Shader* shader;
    Texture* texture;
    GLuint VaoID;
};

#endif
