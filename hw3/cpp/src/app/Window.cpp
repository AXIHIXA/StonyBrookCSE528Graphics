/// STOP. You should not modify this file unless you KNOW what you are doing.

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/Window.h"


Window::Window(int width, int height, const char * title, GLFWmonitor * monitor, GLFWwindow * share)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    pWindow = glfwCreateWindow(width, height, title, monitor, share);

    if (!pWindow)
    {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFW pWindow");
    }

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(pWindow);
        glfwTerminate();
        throw std::runtime_error("failed to initialize GLAD");
    }
}


Window::~Window() noexcept
{
    glfwDestroyWindow(pWindow);
    glfwTerminate();
}
