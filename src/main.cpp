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
#include "window.h"
#include "skybox.h" 

int width = 800;
int height = 800;

std::vector<std::string> faces = {
    "textures/cubemap/space_rt.png",
    "textures/cubemap/space_lf.png",
    "textures/cubemap/space_up.png",
    "textures/cubemap/space_dn.png",
    "textures/cubemap/space_ft.png",
    "textures/cubemap/space_bk.png"
};

int main(){
    // criacao da janela
    Window window(width, height);

    //criacao dos shaders dos planetas e do skybox
    Shader shaderProgram("shaders/VertexShader.vert", "shaders/PlanetFragShader.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    // declarar os VAOs e VBOs
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

    shaderProgram.Activate();
    shaderProgram.SetInt("tex0", 0);

    Skybox skybox(faces, &skyboxShader);
    // adiciona profundidade, sobreposição de objetos
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 4.0f));

    Time time;
    planeta.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
    planeta.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Translate(glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Scale(glm::vec3(0.5f, 0.5f, 0.5f));



    // While principal onde a mágica acontece -----------------------------------------
    while (!glfwWindowShouldClose(window.window)) {
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
        VAO1.Unbind();

        // Renderiza o skybox primeiro
        skybox.Render(camera.view, camera.projection);
        
        // Troca os buffers e processa eventos
        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }



    VAO1.Delete();
    EBO1.Delete();
    VBO1.Delete();

    glfwTerminate();
    return 0;
}
