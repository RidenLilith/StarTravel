#include "window.h"

Window::Window(int wid, int hei)
{
    width = wid;
    height = hei;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(width, height, "StarTravel", NULL, NULL);
    if (window == NULL) {
    std::cout << "Falha ao criar a janela GLFW" << std::endl;
    glfwTerminate();
    }   
    glfwMakeContextCurrent(window);
    // glViewport(0, 0, 940, 400);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao inicializar GLAD" << std::endl;
    }
};