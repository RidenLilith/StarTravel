#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "planet.h"
#include "stb/stb_image.h"
#include "camera.h"
#include "time.h"
#include "texture.h"

int width = 800;
int height = 800;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(width, height, "Janela Teste", NULL, NULL);
    if (window == NULL) {
    std::cout << "Falha ao criar a janela GLFW" << std::endl;
    glfwTerminate();
    return -1;
    }   
    glfwMakeContextCurrent(window);
    // glViewport(0, 0, 940, 400);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // std::cout << planet.indices[0];

    Shader shaderProgram("shaders/VertexShader.vert", "shaders/PlanetFragShader.frag");

    VAO VAO1;
    VAO1.Bind();

    // Texture -----------------------------------
    
    Texture planetaTexture("textures/PlanetA.png", GL_TEXTURE0);
    Texture moonTexture("textures/Moon.jpg", GL_TEXTURE0);

    Planet planeta(10, 10, &planetaTexture, &shaderProgram, VAO1.ID);
    Planet moon(10, 10, &moonTexture, &shaderProgram, VAO1.ID);

    VBO VBO1(planeta.vertices, planeta.vertices.size() * sizeof(float));
    EBO EBO1(planeta.indices, planeta.indices.size() * sizeof(GLuint));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    
    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);

    // adiciona profundidade, sobreposição de objetos
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));

    Time time;
    planeta.Translate(glm::vec3(-1.0f, 0.0f, 0.0f));
    planeta.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Translate(glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    // While principal onde a mágica acontece
    while (!glfwWindowShouldClose(window)) {
        time.Update();
        // Limpa a tela com cor de fundo e depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        // Ativa o shader
        shaderProgram.Activate();
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
        VAO1.Bind();
        //a cada 1 segundo vc rotaciona x.0f graus. Top demais, né?!!!
        planeta.Translate(glm::vec3(0.0f, 0.0f, -0.01f * time.deltaTime));
        planeta.Rotate(-45.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        moon.Rotate(-45.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        planeta.Render();
        moon.Render();

        
        // Troca os buffers e processa eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    EBO1.Delete();
    VBO1.Delete();

    glfwTerminate();
    return 0;
}
