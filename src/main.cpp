#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
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
#include "asteroidSpawner.h"
#include "asteroid.h"

#define PI 3.14159265359f

std::vector<glm::vec3> generateAsteroidTunnelPositions(
    int count, float radius, float zStart, float zEnd, glm::vec3 axis = glm::vec3(0, 0, 1))
{
    std::vector<glm::vec3> positions;
    positions.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distRadius(0.0f, radius);
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * PI);
    std::uniform_real_distribution<float> distZ(zEnd, zStart);

    for (int i = 0; i < count; ++i) {
        float r = distRadius(gen);
        float angle = distAngle(gen);
        float z = distZ(gen);

        // Posição circular ao redor do eixo Z
        float x = r * cos(angle);
        float y = r * sin(angle);

        glm::vec3 position = glm::vec3(x, y, z);
        positions.push_back(position);
    }

    return positions;
}

float tunnelRadius = 15.0f;
float tunnelStartZ = 200.0f;
float tunnelEndZ = 0.0f;
int asteroidCount = 100;


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

    auto asteroidPositions = generateAsteroidTunnelPositions(asteroidCount, tunnelRadius, tunnelStartZ, tunnelEndZ);

    BezierPath cameraPath;
    // Defina os pontos de controle das curvas Bézier (exemplo para 3 segmentos)
    cameraPath.addSegment(
        glm::vec3(0.0f, 0.0f, 100.0f),       // longe, no fundo
        glm::vec3(0.0f, 2.0f, 60.0f),        // curva descendo devagar
        glm::vec3(0.0f, 1.0f, 20.0f),        // curva em direção ao sol
        glm::vec3(0.0f, 0.0f, 5.0f)          // perto do sol
    );

    cameraPath.addSegment(
        glm::vec3(0.0f, 0.0f, 5.0f),         // começa perto do sol
        glm::vec3(-0.5f, 0.5f, 2.5f),        // indo em direção ao planeta
        glm::vec3(-0.5f, 0.2f, 0.5f),        // se aproximando do planeta
        glm::vec3(-0.5f, 0.0f, 0.0f)         // passa pelo planeta
    );

    cameraPath.addSegment(
        glm::vec3(-0.5f, 0.0f, 0.0f),        // sai do planeta
        glm::vec3(0.0f, -2.0f, -20.0f),      // desce um pouco
        glm::vec3(0.0f, -1.0f, -40.0f),      // continua indo para trás
        glm::vec3(0.0f, 0.0f, -60.0f)        // final do trajeto
    );
    
    Texture asteroidTexture("textures/Ceres.jpg", GL_TEXTURE0);
    //criacao dos shaders dos planetas e do skybox
    Shader shaderProgram("shaders/VertexShader.vert", "shaders/PlanetFragShader.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");


    // Asteroid tempAsteroid(glm::vec3(0.0f), 10, 10, &asteroidTexture, &shaderProgram, 0);

    // VAO asteroidVAO;
    // asteroidVAO.Bind();

    // VBO asteroidVBO(tempAsteroid.vertices, tempAsteroid.vertices.size() * sizeof(float));
    // EBO asteroidEBO(tempAsteroid.indices, tempAsteroid.indices.size() * sizeof(GLuint));

    // asteroidVAO.LinkAttrib(asteroidVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    // asteroidVAO.LinkAttrib(asteroidVBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // asteroidVAO.LinkAttrib(asteroidVBO, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    // asteroidVAO.Unbind();
    // asteroidVBO.Unbind();
    // asteroidEBO.Unbind();




    // declarar os VAOs e VBOs
    VAO VAO1;
    VAO1.Bind();


    // Texture -----------------------------------
    
    Texture planetaTexture("textures/Earth.jpg", GL_TEXTURE0);
    Texture moonTexture("textures/Moon.jpg", GL_TEXTURE0);
    Texture sunTex("textures/Sun.jpg", GL_TEXTURE0);

    
    // std::vector<Asteroid> asteroids = generateAsteroidsAlongBezier(
    // cameraPath,           // o caminho Bézier
    // 50,                   // número de asteroides por segmento
    // &asteroidTexture,
    // &shaderProgram,
    // asteroidVAO.ID
    // );

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

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 250.0f));

    shaderProgram.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    Time time;
    planeta.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
    planeta.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Translate(glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    sun.Translate(glm::vec3(0.0f, 0.0f, 5.0f));
    sun.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    sun.Scale(glm::vec3(100.0f, 100.0f, 100.0f));
    moon.Scale(glm::vec3(0.5f, 0.5f, 0.5f));
    planeta.Scale(glm::vec3(3.0f, 3.0f, 3.0f));

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

        camera.Inputs(window.window);
        camera.Matrix(45.0f, 0.1f, 300.0f, shaderProgram, "camMatrix");
        VAO1.Bind();

        //a cada 1 segundo vc rotaciona x.0f graus. Top demais, né?!!!
        planeta.Translate(glm::vec3(0.0f, 0.0f, -0.01f * time.deltaTime));
        moon.OrbitAround(planeta.position, 1.5f, 5.0f, time.deltaTime*1.5, moonOrbitAngle);
        planeta.OrbitAround(sun.position, 150.0f, 5.0f, time.deltaTime, planetOrbitAngle);

        planeta.Rotate(-45.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        sun.Rotate(-10.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        moon.Rotate(-45.0f * time.deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
        


        VAO1.Bind();
        planeta.Render();
        moon.Render();
        sun.Render();
        VAO1.Unbind();
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
