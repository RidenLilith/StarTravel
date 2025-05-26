#ifndef TIME_CLASS_H
#define TIME_CLASS_H
#include <GLFW/glfw3.h>
class Time
{
    float lastFrame = 0.0f;

    public:
        float deltaTime = 0.0f;
        void Update();
};

#endif