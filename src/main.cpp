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
#include "billBoard.h"
#include "cameraController.h"



struct PlanetInfo {
    std::string name;
    glm::vec3* position;
    float radius;
};

int main() {
    float fator = 2.0f;
    int width = 1080, height = 1080;
    Window window(width, height);

    BezierPath cameraPath;
    CameraController pathController;
    // Netuno
    glm::vec3 p0 = glm::vec3(-10000/fator, 0, -80000);
    glm::vec3 p1 = glm::vec3(-10000/fator, 0, -54000);
    glm::vec3 p2 = glm::vec3(-10000/fator, 0, -52000);
    glm::vec3 p3 = glm::vec3(-10000/fator, 0, -50000);
    cameraPath.addSegment(p0, p1, p2, p3);
    pathController.addSegmentDuration(5.0f);
    //Jupiter
    glm::vec3 p4 = glm::vec3(5000, 0, -40000);     // Começando a virar
    glm::vec3 p5 = glm::vec3(4500, 0, -30000);     // Se aproxima, ainda distante
    glm::vec3 p6 = glm::vec3(1000, 0, -25000);     // Entrada anterior
    cameraPath.addSegment(p3, p4, p5, p6);
    pathController.addSegmentDuration(5.0f);

    // Terra(aproximando)
    glm::vec3 p7 = glm::vec3(-2000, -30, -22500);
    glm::vec3 p8 = glm::vec3(-8000, -20, -17500);
    glm::vec3 p9 = glm::vec3(-10000, 0, -12500);
    cameraPath.addSegment(p6, p7, p8, p9);
    pathController.addSegmentDuration(5.0f);

    // (terra, por tras)
    glm::vec3 p10 = glm::vec3(-12000, 10, -10000);
    glm::vec3 p11 = glm::vec3(-16000, 20, -10000);
    glm::vec3 p12 = glm::vec3(-17000, 10, -10000);
    cameraPath.addSegment(p9, p10, p11, p12);
    pathController.addSegmentDuration(2.0f);

    //terra: da a volta e se afasta
    glm::vec3 p13 = glm::vec3(-16000, 10, -2000);   
    glm::vec3 p14 = glm::vec3(-18000, 20, -1500);
    glm::vec3 p15 = glm::vec3(-15000, 10, -2000);
    cameraPath.addSegment(p12, p13, p14, p15);
    pathController.addSegmentDuration(4.0f);

    // vira pro sol
    glm::vec3 p16 = glm::vec3(-15000, 0, -2000);
    glm::vec3 p17 = glm::vec3(-15000, 0, 0);
    glm::vec3 p18 = glm::vec3(0, 0, 10000);
    pathController.addSegmentDuration(3.0f);

    cameraPath.addSegment(p15, p16, p17, p18);

    glm::vec3 p19 = glm::vec3(0, 0, 8000);
    glm::vec3 p20 = glm::vec3(0, 0, 6000);
    glm::vec3 p21 = glm::vec3(0, 0, 4000);
    pathController.addSegmentDuration(6.0f);
    
    cameraPath.addSegment(p18, p19, p20, p21);
    // Se aproxima lentamente
    glm::vec3 p22 = glm::vec3(0, 0, 2000);
    glm::vec3 p23 = glm::vec3(0, 0, 1000);
    glm::vec3 p24 = glm::vec3(0, 0, 300);
    pathController.addSegmentDuration(6.0f);
    
    cameraPath.addSegment(p21, p22, p23, p24);

    Shader billboardShader("shaders/image.vert", "shaders/image.frag");
    Shader shaderProgram("shaders/VertexShader.vert", "shaders/PlanetFragShader.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    VAO VAO1;
    VAO1.Bind();

    Texture texPedro("textures/imagemPedro.png", GL_TEXTURE0);
    Texture texBreno("textures/imagemBreno.png", GL_TEXTURE0);
    Texture texEACH("textures/imagemEACH.png", GL_TEXTURE0);
    float billboardScale = 100.0f;
    float spacing = 150.0f;
    float distanceInFront = -150.0f;  // mais negativo que o centro da cena (Z = 0)

    // Quad para billboards
    std::vector<float> quadVertices = {
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };
    std::vector<GLuint> quadIndices = { 0, 1, 2, 0, 2, 3 };

    VAO VAO_billboard;
    VAO_billboard.Bind();
    VBO quadVBO(quadVertices, quadVertices.size() * sizeof(float));
    EBO quadEBO(quadIndices, quadIndices.size() * sizeof(GLuint));
    VAO_billboard.LinkAttrib(quadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VAO_billboard.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    Billboard billboardPedro(glm::vec3(-spacing, 0.0f, distanceInFront), glm::vec3(25), &texPedro, &billboardShader, VAO_billboard.ID);
    Billboard billboardBreno(glm::vec3(spacing,      0.0f, distanceInFront), glm::vec3(25), &texBreno, &billboardShader, VAO_billboard.ID);
    Billboard billboardEACH(glm::vec3(0.0f,    0.0f, -120), glm::vec3(75), &texEACH, &billboardShader, VAO_billboard.ID);
    VAO_billboard.Unbind();

    Texture texSun("textures/Sun.jpg", GL_TEXTURE0);
    Texture texMercury("textures/Ceres.jpg", GL_TEXTURE0);
    Texture texVenus("textures/Venus.jpg", GL_TEXTURE0);
    Texture texEarth("textures/Earth.jpg", GL_TEXTURE0);
    Texture texMoon("textures/Moon.jpg", GL_TEXTURE0);
    Texture texMars("textures/Mars.jpg", GL_TEXTURE0);
    Texture texJupiter("textures/Jupiter.jpg", GL_TEXTURE0);
    // Texture texSaturn("textures/Saturn.jpg", GL_TEXTURE0);
    // Texture texUranus("textures/Uranus.jpg", GL_TEXTURE0);
    Texture texNeptune("textures/Nepture.jpg", GL_TEXTURE0);

    VAO1.Bind();

    Planet sun(40, 40, &texSun, &shaderProgram, VAO1.ID); sun.emissiveStrength = 1.0f;
    Planet mercury(10, 10, &texMercury, &shaderProgram, VAO1.ID);
    Planet venus(10, 10, &texVenus, &shaderProgram, VAO1.ID);
    Planet earth(10, 10, &texEarth, &shaderProgram, VAO1.ID);
    Planet moon(10, 10, &texMoon, &shaderProgram, VAO1.ID);
    Planet mars(10, 10, &texMars, &shaderProgram, VAO1.ID);
    Planet jupiter(10, 10, &texJupiter, &shaderProgram, VAO1.ID);
    // Planet saturn(10, 10, &texSaturn, &shaderProgram, VAO1.ID);
    // Planet uranus(10, 10, &texUranus, &shaderProgram, VAO1.ID);
    Planet neptune(10, 10, &texNeptune, &shaderProgram, VAO1.ID);

    sun.Scale(glm::vec3(1000.0f));              // Sol: enorme
    mercury.Scale(glm::vec3(15.0f*4.0f));           // 0.38 * 40
    venus.Scale(glm::vec3(35.0f*4.0f));             // 0.95 * 40
    earth.Scale(glm::vec3(800.0f));             // referência
    moon.Scale(glm::vec3(40.0f));              // 0.27 * 40
    mars.Scale(glm::vec3(100.0f));              // 0.53 * 40
    jupiter.Scale(glm::vec3(1200.0f));          // 10.97 * 40
    neptune.Scale(glm::vec3(1000.0f));          // 3.88 * 40

    sun.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
    mercury.Translate(glm::vec3(4500.0f/fator, 15.0f, -2000.0f/fator));
    venus.Translate(glm::vec3(-5000.0f/fator, 20.0f, -5000.0f/fator));
    earth.Translate(glm::vec3(-15000, 5.0f, -5000));
    moon.Translate(glm::vec3(-230.0f/fator, 5.0f, -3080.0f/fator));
    mars.Translate(glm::vec3(-25000.0f/fator, 0.0f, 20000.0f/fator));
    jupiter.Translate(glm::vec3(-10000.0f/fator, -10.0f, -50000.0f/fator));
    neptune.Translate(glm::vec3(-5000, -50.0f, -40000));

    sun.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    mercury.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    venus.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    mars.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    jupiter.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    neptune.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    earth.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    moon.Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));


    std::vector<glm::vec3*> cameraTargets = {
        &neptune.position, // Segmento 0
        &jupiter.position,   // Segmento 1
        &earth.position,   // Segmento 2
        &earth.position,   // Segmento 3
        &earth.position,     // Segmento 4
        &earth.position,     // Segmento 5
        &sun.position      // Segmento 6
    };



    VBO VBO1(earth.vertices, earth.vertices.size() * sizeof(float));
    EBO EBO1(earth.indices, earth.indices.size() * sizeof(GLuint));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    VAO VAO2;
    VAO2.Bind();

    std::vector<std::string> faces = {
        "textures/cubemap/space_bk.png",
        "textures/cubemap/space_bk.png",
        "textures/cubemap/space_bk.png",
        "textures/cubemap/space_bk.png",
        "textures/cubemap/space_bk.png",
        "textures/cubemap/space_bk.png"
    };
    Skybox skybox(faces, &skyboxShader);
    VBO VBO2(skybox.vertices, skybox.vertices.size());

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(3000.0f, 2000.0f, 3000.0f));
    camera.view = glm::lookAt(camera.Position, sun.position, glm::vec3(0.0f, 1.0f, 0.0f));
    Time time;

    // Ângulos separados para cada planeta
    float mercuryAngle = 45.0f;
    float venusAngle = 20.0f;
    float earthAngle = 35.0f;
    float moonAngle = 0.0f;
    float marsAngle = 75.0f;
    float jupiterAngle = 70.0f;
    float neptuneAngle = 66.0f;

    float elapsedTime = 0.0f;
    float travelTime = 15.0f;
    bool cameraLocked = true;

    while (!glfwWindowShouldClose(window.window)) {
        time.Update();
        elapsedTime += time.deltaTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        shaderProgram.Activate();
        shaderProgram.SetVec3("lightPos", glm::vec3(sun.model[3]));
        shaderProgram.SetVec3("viewPos", camera.Position);

        if (cameraLocked) {
            // Corrigido: declara antes de usar
            int currentSegment = pathController.getCurrentSegment(elapsedTime);
            float localT = pathController.getLocalT(elapsedTime);

            glm::vec3 currentPosition = cameraPath.evaluate(currentSegment, localT);
            camera.Position = currentPosition;

            glm::vec3 target;
            if (currentSegment < cameraTargets.size()) {
                glm::vec3* target = cameraTargets[currentSegment];
                camera.SmoothLookAt(*target - currentPosition, 0.1f);
            }

            if (elapsedTime >= pathController.getTotalTime()) {
                cameraLocked = false;
                camera.view = glm::lookAt(camera.Position, sun.position, glm::vec3(0, 1, 0));
            }
        }

        camera.Matrix(45.0f, 0.1f, 70000.0f, shaderProgram, "camMatrix");

        // 
        moon.OrbitAround(earth.position, 6.0f, 300.0f, time.deltaTime, moonAngle);

        // 
        sun.Rotate(-3.0f * time.deltaTime, glm::vec3(0, 0, 1));
        earth.Rotate(-5.0f * time.deltaTime, glm::vec3(0, 0, 1));
        jupiter.Rotate(-8.0f * time.deltaTime, glm::vec3(0, 0, 1));
        neptune.Rotate(-5.0f * time.deltaTime, glm::vec3(0, 0, 1));
        mercury.Rotate(-4.0f * time.deltaTime, glm::vec3(0, 0, 1));

        billboardPedro.Rotate(45.0f * time.deltaTime, glm::vec3(0, 1, 0));
        billboardBreno.Rotate(45.0f * time.deltaTime, glm::vec3(0, 1, 0));
        // billboardEACH.Rotate(10.0f * time.deltaTime, glm::vec3(0, 1, 0));

        VAO1.Bind();
        sun.Render();
        mercury.Render();
        venus.Render();
        earth.Render();
        moon.Render();
        mars.Render();
        jupiter.Render();
        neptune.Render();
        VAO1.Unbind();

        billboardShader.Activate();
        VAO_billboard.Bind();
        camera.Matrix(45.0f, 0.1f, 70000.0f, billboardShader, "camMatrix");
        billboardPedro.Render();
        billboardBreno.Render();
        billboardEACH.Render();
        VAO_billboard.Unbind();

        skybox.Render(camera.view, camera.projection);

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }


    VAO1.Delete();
    EBO1.Delete();
    VBO1.Delete();

    glfwTerminate();
    return 0;
}
