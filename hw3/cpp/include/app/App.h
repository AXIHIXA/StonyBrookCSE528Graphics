#ifndef APP_H
#define APP_H

#include <memory>

#include <glm/glm.hpp>

#include "app/Window.h"
#include "util/Camera.h"


class Shader;
class Renderable;


class App : private Window
{
public:
    static App & getInstance();

    void run();

private:
    static void cursorPosCallback(GLFWwindow *, double, double);
    static void framebufferSizeCallback(GLFWwindow *, int, int);
    static void keyCallback(GLFWwindow *, int, int, int, int);
    static void mouseButtonCallback(GLFWwindow *, int, int, int);
    static void scrollCallback(GLFWwindow *, double, double);

    static void perFrameTimeLogic(GLFWwindow *);
    static void processKeyInput(GLFWwindow *);

    // from CMakeLists.txt, compile definition
    static constexpr char kWindowName[] {WINDOW_NAME};
    static constexpr int kWindowWidth {1000};
    static constexpr int kWindowHeight {1000};

private:
    App();

    void initializeShadersAndObjects();

    void render();

    // Shaders.
    std::unique_ptr<Shader> pLineShader;
    std::unique_ptr<Shader> pMeshShader;
    std::unique_ptr<Shader> pSphereShader;

    // Objects to render.
    std::vector<std::unique_ptr<Renderable>> shapes;

    // Viewing
    Camera camera {{0.0f, 0.0f, 10.0f}};
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    glm::vec3 lightColor {1.0f, 1.0f, 1.0f};
    glm::vec3 lightPos {10.0f, -10.0f, 10.0f};

    // Frontend GUI
    double timeElapsedSinceLastFrame {0.0};
    double lastFrameTimeStamp {0.0};

    bool mousePressed {false};
    glm::dvec2 mousePos {0.0, 0.0};

    // Used for camera movement from mouse dragging.
    // Note lastMouseLeftClickPos is different from lastMouseLeftPressPos.
    // If you press left button (and hold it there) and move the mouse,
    // lastMouseLeftPressPos gets updated to the current mouse position
    // (while lastMouseLeftClickPos, if there is one, remains the original value).
    glm::dvec2 lastMouseLeftClickPos {0.0, 0.0};
    glm::dvec2 lastMouseLeftPressPos {0.0, 0.0};
};


#endif  // APP_H
