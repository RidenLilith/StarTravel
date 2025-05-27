#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Shader;

class Skybox {
public:
    GLuint VAO, VBO, cubemapTexture;
    Shader* shader;
    std::vector<float> vertices;
    Skybox(const std::vector<std::string>& faces, Shader* skyboxShader);
    void Render(const glm::mat4& view, const glm::mat4& projection);
    void Delete();

private:
    GLuint LoadCubemap(const std::vector<std::string>& faces);
};

#endif