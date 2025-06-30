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
#include "Bezier.h"
#include "bezierPath.h"

int width = 800;
int height = 800;

std::vector<std::string> faces = {
    "textures/cubemap/Stars.jpg",
    "textures/cubemap/Stars.jpg",
    "textures/cubemap/Stars.jpg",
    "textures/cubemap/Stars.jpg",
    "textures/cubemap/Stars.jpg",
    "textures/cubemap/Stars.jpg"
};


float bezierTime = 0.0f;
const float bezierDuration = 20.0f; // duração total para percorrer todas as curvas


int main(){
    // criacao da janela
    Window window(width, height);

    
    BezierPath cameraPath;
    // Defina os pontos de controle das curvas Bézier (exemplo para 3 segmentos)
    cameraPath.addSegment(
        glm::vec3(0.0f, 2.0f, 100.0f), // p0
        glm::vec3(40.0f, 0.0f, 60.0f), // p1
        glm::vec3(-40.0f, 0.5f, 2.0f), // p2
        glm::vec3(0.0f, 0.0f, 0.0f)     // p3 (fim do 1o segmento)
    );

    cameraPath.addSegment(
        glm::vec3(0.0f, 0.0f, 0.0f),   // p0 (de onde parou no segmento anterior)
        glm::vec3(20.0f, 10.0f, -20.0f), // p1
        glm::vec3(-20.0f, 5.0f, -40.0f), // p2
        glm::vec3(0.0f, 0.0f, -60.0f)     // p3 (fim do 2o segmento)
    );

    cameraPath.addSegment(
        glm::vec3(0.0f, 0.0f, -60.0f), // p0 (início do 3o segmento)
        glm::vec3(0.0f, 20.0f, -80.0f), // p1
        glm::vec3(10.0f, 10.0f, -90.0f), // p2
        glm::vec3(0.0f, 0.0f, -100.0f)   // p3 (final da curva)
    );

    //criacao dos shaders dos planetas e do skybox
    Shader shaderProgram("shaders/VertexShader.vert", "shaders/PlanetFragShader.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    // declarar os VAOs e VBOs
    VAO VAO1;
    VAO1.Bind();


    // Texture -----------------------------------
    
    Texture planetaTexture("textures/Earth.jpg", GL_TEXTURE0);
    Texture moonTexture("textures/Moon.jpg", GL_TEXTURE0);
    Texture sunTex("textures/Sun.jpg", GL_TEXTURE0);
    
    
    Planet planeta(10, 10, &planetaTexture, &shaderProgram, VAO1.ID);
    Planet moon(10, 10, &moonTexture, &shaderProgram, VAO1.ID);

    Planet sun(20, 20, &sunTex, &shaderProgram, VAO1.ID);
    sun.emissiveStrength = 1.0f; // brilha totalmente

    VBO VBO1(planeta.vertices, planeta.vertices.size() * sizeof(float));
    EBO EBO1(planeta.indices, planeta.indices.size() * sizeof(GLuint));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    shaderProgram.Activate();
    shaderProgram.SetInt("tex0", 0);



    VAO VAO2;
    VAO2.Bind();

    Skybox skybox(faces, &skyboxShader);
    VBO VBO2(skybox.vertices, skybox.vertices.size());
    // adiciona profundidade, sobreposição de objetos
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 10.0f));

    shaderProgram.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    Time time;
    planeta.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
    planeta.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Translate(glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    sun.Translate(glm::vec3(0.0f, 0.0f, 5.0f));
    sun.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    float moonOrbitAngle = 0.0f;
    float planetOrbitAngle = 0.0f;


    float bezierTime = 0.0f;
    const float bezierDuration = 10.0f; // segundos que dura o movimento da câmera na curva

    // While principal onde a mágica acontece -----------------------------------------
    while (!glfwWindowShouldClose(window.window)) {
        time.Update();
        // Limpa a tela com cor de fundo e depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Ativa o shader
        shaderProgram.Activate();
        glm::vec3 sunWorldPos = glm::vec3(sun.model[3]);

        // Envia a posição da luz e a posição da câmera para o shader
        shaderProgram.SetVec3("lightPos", sunWorldPos);
        shaderProgram.SetVec3("viewPos", camera.Position);
        // float angle = (float)glfwGetTime(); // tempo em segundos
        // camera.Orientation = glm::normalize(glm::vec3(sin(angle/10), 0.0f, -cos(angle/10)));
        if (bezierTime < bezierDuration) {
            float t = bezierTime / bezierDuration;  // Normaliza 0 a 1 para todo o caminho
            camera.Position = cameraPath.evaluate(t);

            // Olha para o centro do sistema solar (exemplo)
            camera.Orientation = glm::normalize(glm::vec3(0.0f) - camera.Position);

            bezierTime += time.deltaTime;
        } else {
            camera.Inputs(window.window);  // controle manual liberado depois da curva
        }
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
        VAO1.Bind();
        //a cada 1 segundo vc rotaciona x.0f graus. Top demais, né?!!!
        planeta.Translate(glm::vec3(0.0f, 0.0f, -0.01f * time.deltaTime));
        moon.OrbitAround(planeta.position, 1.5f, 30.0f, time.deltaTime*1.5, moonOrbitAngle);
        planeta.OrbitAround(sun.position, 5.0f, 30.0f, time.deltaTime, planetOrbitAngle);

        planeta.Rotate(-45.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        moon.Rotate(-45.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        
        planeta.Render();
        moon.Render();
        sun.Render();
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
