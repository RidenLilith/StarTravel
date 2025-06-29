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
        float emissiveStrength = 0.0f;
        glm::mat4 rotation = glm::mat4(1.0f);
        glm::vec3 position = glm::vec3(0.0f); // posição atual

        glm::vec3 currentScale = glm::vec3(1.0f); // escala padrão

        glm::mat4 model = glm::mat4(1.0f);

        Planet(int stacks, int sectors, Texture* tex, Shader* sha, GLuint ID);
        
        void OrbitAround(glm::vec3 center, float radius, float speed, float deltaTime, float& angle);
        void Translate(glm::vec3 offset);
        void Rotate(float angle, glm::vec3 axis);
        void Scale(glm::vec3 scaleFactor);
        void Render();
};

#endif