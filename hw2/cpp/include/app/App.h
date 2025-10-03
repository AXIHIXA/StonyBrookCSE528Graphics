#ifndef APP_H
#define APP_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "app/Window.h"


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

    void render();

    // Shaders.
    // In principle, a shader could be reused across multiple objects.
    // Thus, these shaders are not designed as members of object classes.
    std::unique_ptr<Shader> pTriangleShader {nullptr};
    std::unique_ptr<Shader> pCircleShader {nullptr};

    // Objects to render.
    std::vector<std::unique_ptr<Renderable>> shapes;

    // Object attributes affected by GUI.
    bool animationEnabled {true};

    // Frontend GUI
    double timeElapsedSinceLastFrame {0.0};
    double lastFrameTimeStamp {0.0};

    bool mousePressed {false};
    glm::dvec2 mousePos {0.0, 0.0};

    // Note lastMouseLeftClickPos is different from lastMouseLeftPressPos.
    // If you press left button (and hold it there) and move the mouse,
    // lastMouseLeftPressPos gets updated to the current mouse position
    // (while lastMouseLeftClickPos, if there is one, remains the original value).
    glm::dvec2 lastMouseLeftClickPos {0.0, 0.0};
    glm::dvec2 lastMouseLeftPressPos {0.0, 0.0};
};


#endif  // APP_H
