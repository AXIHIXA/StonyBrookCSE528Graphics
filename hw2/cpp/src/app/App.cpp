#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.h"
#include "shape/Circle.h"
#include "shape/Triangle.h"
#include "util/Shader.h"


App & App::getInstance()
{
    static App instance;
    return instance;
}


void App::run()
{
    while (!glfwWindowShouldClose(pWindow))
    {
        // Per-frame logic
        perFrameTimeLogic(pWindow);
        processKeyInput(pWindow);

        // Send render commands to OpenGL server
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        // Check and call events and swap the buffers
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}


void App::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    app.mousePos.x = xpos;
    app.mousePos.y = App::kWindowHeight - ypos;

    if (app.mousePressed)
    {
        // // Note: Must calculate offset first, then update lastMouseLeftPressPos.
        // glm::dvec2 offset = app.mousePos - app.lastMouseLeftPressPos;
        app.lastMouseLeftPressPos = app.mousePos;
    }
}


void App::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void App::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        app.animationEnabled = !app.animationEnabled;
    }
}


void App::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            app.mousePressed = true;
            app.lastMouseLeftClickPos = app.mousePos;
            app.lastMouseLeftPressPos = app.mousePos;
        }
        else if (action == GLFW_RELEASE)
        {
            app.mousePressed = false;

            #ifdef DEBUG_MOUSE_POS
            std::cout << "[ " << app.mousePos.x << ' ' << app.mousePos.y << " ]\n";
            #endif
        }
    }
}


void App::scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{

}


void App::perFrameTimeLogic(GLFWwindow * window)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    double currentFrame = glfwGetTime();
    app.timeElapsedSinceLastFrame = currentFrame - app.lastFrameTimeStamp;
    app.lastFrameTimeStamp = currentFrame;
}


void App::processKeyInput(GLFWwindow * window)
{

}


App::App() : Window(kWindowWidth, kWindowHeight, kWindowName, nullptr, nullptr)
{
    // GLFW boilerplate.
    glfwSetWindowUserPointer(pWindow, this);
    glfwSetCursorPosCallback(pWindow, cursorPosCallback);
    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);
    glfwSetKeyCallback(pWindow, keyCallback);
    glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);
    glfwSetScrollCallback(pWindow, scrollCallback);

    // Global OpenGL pipeline settings
    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
    glPointSize(1.0f);

    // Initialize shaders and objects-to-render;
    pTriangleShader = std::make_unique<Shader>("src/shader/triangle.vert.glsl",
                                               "src/shader/triangle.frag.glsl");
    pCircleShader = std::make_unique<Shader>("src/shader/circle.vert.glsl",
                                             "src/shader/circle.tesc.glsl",
                                             "src/shader/circle.tese.glsl",
                                             "src/shader/circle.frag.glsl");

    shapes.emplace_back(
            std::make_unique<Triangle>(
                    pTriangleShader.get(),
                    std::vector<Triangle::Vertex> {
                            // Vertex coordinate (screen-space coordinate), Vertex color
                            {{200.0f, 326.8f}, {1.0f, 0.0f, 0.0f}},
                            {{800.0f, 326.8f}, {0.0f, 1.0f, 0.0f}},
                            {{500.0f, 846.4f}, {0.0f, 0.0f, 1.0f}},
                    }
            )
    );

    shapes.emplace_back(
            std::make_unique<Circle>(
                    pCircleShader.get(),
                    std::vector<glm::vec3> {
                            // Coordinate (x, y) of the center and the radius (screen-space)
                            {200.0f, 326.8f, 200.0f},
                            {800.0f, 326.8f, 300.0f},
                            {500.0f, 846.4f, 400.0f}
                    }
            )
    );
}


void App::render()
{
    auto t = static_cast<float>(timeElapsedSinceLastFrame);

    // Update all shader uniforms.
    pTriangleShader->use();
    pTriangleShader->setFloat("windowWidth", kWindowWidth);
    pTriangleShader->setFloat("windowHeight", kWindowHeight);

    pCircleShader->use();
    pCircleShader->setFloat("windowWidth", kWindowWidth);
    pCircleShader->setFloat("windowHeight", kWindowHeight);

    // Render all shapes.
    for (auto & s : shapes)
    {
        s->render(t, animationEnabled);
    }
}
