#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Camera::Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{

    view = glm::lookAt(Position, Position + Orientation, Up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));

}

void Camera::Inputs(GLFWwindow* window)
{
    speed = 0.2f;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += (speed*40.0f) * Orientation;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += (speed*40.0f) * -glm::normalize(glm::cross(Orientation, Up));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += (speed*40.0f) * -Orientation;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += (speed*40.0f) * glm::normalize(glm::cross(Orientation, Up));
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        float rotx = sensitivity * (float)(mouseY - (height/2)) / height;
        float roty = sensitivity * (float)(mouseX - (height/2)) / height;
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotx), glm::normalize(glm::cross(Orientation, Up)));
        if(!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))
        {
            Orientation = newOrientation;
        }
        Orientation = glm::rotate(Orientation, glm::radians(-roty), Up);
    }
    else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // imprime posição da câmera quando a barra de espaço é pressionada (para debug)
    static bool spaceWasPressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!spaceWasPressed)
        {
            std::cout << "Camera Position: x=" << Position.x
                      << ", y=" << Position.y
                      << ", z=" << Position.z << std::endl;
            spaceWasPressed = true;
        }
    }
    else
    {
        spaceWasPressed = false;
    }
}

void Camera::SmoothLookAt(const glm::vec3& targetDirection, float turnSpeed)
{
    glm::vec3 normalizedTarget = glm::normalize(targetDirection);
    Orientation = glm::normalize(glm::mix(Orientation, normalizedTarget, turnSpeed));
}