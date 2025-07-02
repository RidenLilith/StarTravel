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
    // Se aproxima do sistema solar 3s
    glm::vec3 p0 = glm::vec3(-10000/fator, 0, -80000);
    glm::vec3 p1 = glm::vec3(-10000/fator, 0, -54000);
    glm::vec3 p2 = glm::vec3(-10000/fator, 0, -52000);
    glm::vec3 p3 = glm::vec3(-10000/fator, 0, -50000);
    cameraPath.addSegment(p0, p1, p2, p3);

    glm::vec3 p4 = glm::vec3(5000, 0, -40000);     // Começando a virar
    glm::vec3 p5 = glm::vec3(4500, 0, -30000);     // Se aproxima, ainda distante
    glm::vec3 p6 = glm::vec3(3000, 0, -25000);     // Entrada anterior
    cameraPath.addSegment(p3, p4, p5, p6);

    // Começa a circular (passa pelo planeta 1) 3s
    glm::vec3 p7 = glm::vec3(-2000, -30, -22500);
    glm::vec3 p8 = glm::vec3(-8000, -20, -17500);
    glm::vec3 p9 = glm::vec3(-10000, 0, -12500);
    cameraPath.addSegment(p6, p7, p8, p9);

    // // //Continua circulando (passa pelo paneta 2) 3s
    glm::vec3 p10 = glm::vec3(-12000, 10, -10000);
    glm::vec3 p11 = glm::vec3(-14000, 20, -10000);
    glm::vec3 p12 = glm::vec3(-15000, 10, -10000);
    cameraPath.addSegment(p9, p10, p11, p12);

    //dar a volta na terra
    glm::vec3 p13 = glm::vec3(-16000, 10, -2000);   
    glm::vec3 p14 = glm::vec3(-18000, 20, -1500);
    glm::vec3 p15 = glm::vec3(-15000, 10, -2000);
    cameraPath.addSegment(p12, p13, p14, p15);

    // //Contorna o sol 3s
    glm::vec3 p16 = glm::vec3(-20000, 0, -2000);
    glm::vec3 p17 = glm::vec3(-20000, 0, 0);
    glm::vec3 p18 = glm::vec3(0, 0, 10000);

    cameraPath.addSegment(p15, p16, p17, p18);

    glm::vec3 p19 = glm::vec3(0, 0, 8000);
    glm::vec3 p20 = glm::vec3(0, 0, 6000);
    glm::vec3 p21 = glm::vec3(0, 0, 4000);
    
    cameraPath.addSegment(p18, p19, p20, p21);

    glm::vec3 p22 = glm::vec3(0, 0, 2000);
    glm::vec3 p23 = glm::vec3(0, 0, 1000);
    glm::vec3 p24 = glm::vec3(0, 0, 200);
    
    cameraPath.addSegment(p21, p22, p23, p24);

    Shader shaderProgram("shaders/VertexShader.vert", "shaders/PlanetFragShader.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    VAO VAO1;
    VAO1.Bind();

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


    std::vector<PlanetInfo> planetInfos = {
        {"Sun", &sun.position, 300.0f},
        {"Mercury", &mercury.position, 15.0f * 4.0f},
        {"Venus", &venus.position, 35.0f * 4.0f},
        {"Earth", &earth.position, 40.0f * 4.0f},
        {"Moon", &moon.position, 10.0f * 4.0f},
        {"Mars", &mars.position, 25.0f * 4.0f},
        {"Jupiter", &jupiter.position, 440.0f},
        {"Neptune", &neptune.position, 155.0f}
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

        float t = glm::clamp(elapsedTime / travelTime, 0.0f, 1.0f);
        glm::vec3 currentPosition = cameraPath.evaluate(t);
        if (cameraLocked) {
            glm::vec3 currentPosition = cameraPath.evaluate(t);
            camera.Position = currentPosition;

            glm::vec3 target;

            // Determina para onde olhar com base no segmento atual
            if (t < 1.0f / 6.0f) {
                target = jupiter.position;  // 1º segmento
            } else if (t < 5.0f / 6.0f) {
                target = earth.position;    // do 2º ao penúltimo segmento
            } else {
                target = sun.position;      // último segmento
            }

            camera.SmoothLookAt(target - currentPosition, 0.1f);

            if (elapsedTime >= travelTime) {
                cameraLocked = false;
                // Ajuste final da câmera ao fim da animação
                camera.view = glm::lookAt(camera.Position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            }
        }
        
        camera.Matrix(45.0f, 0.1f, 70000.0f, shaderProgram, "camMatrix");
        
        // 🌙 LUA
        moon.OrbitAround(earth.position, 6.0f, 300.0f, time.deltaTime, moonAngle);

        // 🌐 ROTAÇÕES PRÓPRIAS
        sun.Rotate(-3.0f * time.deltaTime, glm::vec3(0, 0, 1));
        earth.Rotate(-5.0f * time.deltaTime, glm::vec3(0, 0, 1));
        jupiter.Rotate(-8.0f * time.deltaTime, glm::vec3(0, 0, 1));
        neptune.Rotate(-5.0f * time.deltaTime, glm::vec3(0, 0, 1));
        mercury.Rotate(-4.0f * time.deltaTime, glm::vec3(0, 0, 1));
        neptune.Rotate(-5.0f * time.deltaTime, glm::vec3(0, 0, 1));

        VAO1.Bind();
        sun.Render();
        mercury.Render();
        venus.Render();
        earth.Render();
        moon.Render();
        mars.Render();
        jupiter.Render();
        // saturn.Render();
        // uranus.Render();
        neptune.Render();
        VAO1.Unbind();

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
