/// STOP. You should not modify this file unless you KNOW what you are doing.

#ifndef WINDOW_H
#define WINDOW_H


class GLFWmonitor;
class GLFWwindow;


class Window
{
public:
    Window() = delete;
    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window & operator=(const Window &) = delete;
    Window & operator=(Window &&) = delete;

protected:
    Window(int width, int height, const char * title, GLFWmonitor * monitor, GLFWwindow * share);
    ~Window() noexcept;

    GLFWwindow * pWindow {nullptr};
};


#endif  // WINDOW_H
