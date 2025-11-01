#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/App.h"
#include "shape/Line.h"
#include "shape/Mesh.h"
#include "shape/Sphere.h"
#include "shape/Tetrahedron.h"
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        // Note: Must calculate offset first, then update lastMouseLeftPressPos.
        glm::dvec2 offset = app.mousePos - app.lastMouseLeftPressPos;
        app.lastMouseLeftPressPos = app.mousePos;
        app.camera.processMouseMovement(offset.x, offset.y);
    }
}


void App::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void App::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{

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
        }
    }
}


void App::scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));
    app.camera.processMouseScroll(yoffset);
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
    // Camera control
    App & app = *reinterpret_cast<App *>(glfwGetWindowUserPointer(window));

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kLeft, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kRight, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kBackWard, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kForward, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kUp, app.timeElapsedSinceLastFrame);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        app.camera.processKeyboard(Camera::kDown, app.timeElapsedSinceLastFrame);
    }
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
    glLineWidth(2.0f);
    glPointSize(1.0f);
    glEnable(GL_DEPTH_TEST);

    initializeShadersAndObjects();
}


void App::initializeShadersAndObjects()
{
    pLineShader = std::make_unique<Shader>("src/shader/line.vert.glsl",
                                           "src/shader/line.frag.glsl");

    pMeshShader = std::make_unique<Shader>("src/shader/mesh.vert.glsl",
                                           "src/shader/phong.frag.glsl");

    pSphereShader = std::make_unique<Shader>("src/shader/sphere.vert.glsl",
                                             "src/shader/sphere.tesc.glsl",
                                             "src/shader/sphere.tese.glsl",
                                             "src/shader/phong.frag.glsl");

    shapes.emplace_back(
            std::make_unique<Line>(
                    pLineShader.get(),
                    std::vector<Line::Vertex> {
                            {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                            {{3.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                            {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                            {{0.0f, 3.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                            {{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 1.0f}},
                    },
                    glm::mat4(1.0f)
            )
    );

    shapes.emplace_back(
            std::make_unique<Tetrahedron>(
                    pMeshShader.get(),
                    "var/tetrahedron.txt",
                    glm::translate(glm::mat4(1.0f), {-2.0f, 0.0f, 0.0f})
            )
    );

    shapes.emplace_back(
            std::make_unique<Mesh>(
                    pMeshShader.get(),
                    std::vector<Mesh::Vertex> {
                            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
                            {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
                            {{0.0f,  0.5f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                    },
                    glm::rotate(
                            glm::translate(glm::mat4(1.0f), {2.0f, 0.0f, 0.0f}),
                            glm::radians(45.0f), {0.0f, 1.0f, 0.0f}
                    )
            )
    );

    shapes.emplace_back(
            std::make_unique<Sphere>(
                    pSphereShader.get(),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    1.0f,
                    glm::vec3(1.0f, 0.5f, 0.31f),
                    glm::mat4(1.0f)
            )
    );
}


void App::render()
{
    auto t = static_cast<float>(timeElapsedSinceLastFrame);

    // Update shader uniforms.
    view = camera.getViewMatrix();
    projection = glm::perspective(glm::radians(camera.zoom),
                                  static_cast<GLfloat>(kWindowWidth) / static_cast<GLfloat>(kWindowHeight),
                                  0.01f,
                                  100.0f);

    pLineShader->use();
    pLineShader->setMat4("view", view);
    pLineShader->setMat4("projection", projection);

    pMeshShader->use();
    pMeshShader->setMat4("view", view);
    pMeshShader->setMat4("projection", projection);
    pMeshShader->setVec3("ViewPos", camera.position);
    pMeshShader->setVec3("lightPos", lightPos);
    pMeshShader->setVec3("lightColor", lightColor);

    pSphereShader->use();
    pSphereShader->setMat4("view", view);
    pSphereShader->setMat4("projection", projection);
    pSphereShader->setVec3("ViewPos", camera.position);
    pSphereShader->setVec3("lightPos", lightPos);
    pSphereShader->setVec3("lightColor", lightColor);

    // Render.
    for (auto & s : shapes)
    {
        s->render(t);
    }
}
