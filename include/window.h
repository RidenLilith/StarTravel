#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Window
{
    public:
        GLFWwindow* window;
        int width;
        int height;
        Window(int wid, int hei);
};


#endif