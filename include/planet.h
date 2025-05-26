#ifndef PLANET_CLASS_H
#define PLANET_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "shaderClass.h"

class Planet
{
    public:
        std::vector<float> vertices;
        std::vector<GLuint> indices;
        Texture* texture;
        Shader* shader;
        GLuint VaoID;

        glm::mat4 model = glm::mat4(1.0f);

        Planet(int stacks, int sectors, Texture* tex, Shader* sha, GLuint ID);

        void Translate(glm::vec3 offset);
        void Rotate(float angle, glm::vec3 axis);
        void Scale(glm::vec3 scaleFactor);
        void Render();
};

#endif