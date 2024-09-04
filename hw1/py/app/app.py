import copy

from OpenGL.GL import *
from glfw.GLFW import *
from glfw import _GLFWwindow as GLFWwindow
import glm

from .window import Window
from shape import Pixel, Renderable
from util import Shader


class App(Window):
    def __init__(self):
        self.windowName: str = 'hw1'
        self.windowWidth: int = 1000
        self.windowHeight: int = 1000
        super().__init__(self.windowWidth, self.windowHeight, self.windowName)
        
        # GLFW boilerplate.
        glfwSetWindowUserPointer(self.window, self)
        glfwSetCursorPosCallback(self.window, self.__cursorPosCallback)
        glfwSetFramebufferSizeCallback(self.window, self.__framebufferSizeCallback)
        glfwSetKeyCallback(self.window, self.__keyCallback)
        glfwSetMouseButtonCallback(self.window, self.__mouseButtonCallback)
        glfwSetScrollCallback(self.window, self.__scrollCallback)

        # Global OpenGL pipeline settings.
        glViewport(0, 0, self.windowWidth, self.windowHeight)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        glLineWidth(1.0)
        glPointSize(1.0)
        
        # Program context.
        
        # Shaders.
        self.pixelShader: Shader = \
            Shader(vert='shader/pixel.vert.glsl', 
                   tesc=None,
                   tese=None,
                   frag='shader/pixel.frag.glsl')
                   
        # Shapes.
        self.shapes: list[Renderable] = []
        self.shapes.append(Pixel(self.pixelShader))

        # Frontend GUI
        self.showPreview: bool = False
        
        self.timeElapsedSinceLastFrame: float = 0.0
        self.lastFrameTimeStamp: float = 0.0
        self.mousePressed: bool = False
        self.mousePos: glm.dvec2 = glm.dvec2(0.0, 0.0)
        
        self.debugMousePos: bool = False
        
        # Note lastMouseLeftClickPos is different from lastMouseLeftPressPos.
        # If you press left button (and hold it there) and move the mouse,
        # lastMouseLeftPressPos gets updated to the current mouse position
        # (while lastMouseLeftClickPos, if there is one, remains the original value).
        self.lastMouseLeftClickPos: glm.dvec2 = glm.dvec2(0.0, 0.0)
        self.lastMouseLeftPressPos: glm.dvec2 = glm.dvec2(0.0, 0.0)
        
    def run(self) -> None:
        while not glfwWindowShouldClose(self.window):
            # Per-frame logic
            self.__perFrameTimeLogic(self.window)
            self.__processKeyInput(self.window)

            # Send render commands to OpenGL server
            glClearColor(0.2, 0.3, 0.3, 1.0)
            glClear(GL_COLOR_BUFFER_BIT)

            self.__render()

            # Check and call events and swap the buffers
            glfwSwapBuffers(self.window)
            glfwPollEvents()
    
    @staticmethod
    def __cursorPosCallback(window: GLFWwindow, xpos: float, ypos: float) -> None:
        app: App = glfwGetWindowUserPointer(window)

        app.mousePos.x = xpos;
        app.mousePos.y = app.windowHeight - ypos;

        if app.mousePressed:
            # # Note: Must calculate offset first, then update lastMouseLeftPressPos.
            # # Also must invoke copy explicitly. 
            # # C++: copy assign is copy; Python: it's reference!
            # glm::dvec2 offset = app.mousePos - app.lastMouseLeftPressPos;
            app.lastMouseLeftPressPos = copy.deepcopy(app.mousePos)
        
        # Display a preview line which moves with the mouse cursor iff.
        # the most-recent mouse click is left click.
        # showPreview is controlled by mouseButtonCallback.
        if app.showPreview:
            pixel: Pixel = app.shapes[0]

            x0 = int(app.lastMouseLeftPressPos.x)
            y0 = int(app.lastMouseLeftPressPos.y)
            x1 = int(app.mousePos.x)
            y1 = int(app.mousePos.y)

            pixel.path.clear()
            App.__bresenhamLine(pixel.path, x0, y0, x1, y1);
            pixel.dirty = True
    
    @staticmethod
    def __framebufferSizeCallback(window: GLFWwindow, width: int, height: int) -> None:
        glViewport(0, 0, width, height)
    
    @staticmethod
    def __keyCallback(window: GLFWwindow, key: int, scancode: int, action: int, mods: int) -> None:
        pass
        
    @staticmethod
    def __mouseButtonCallback(window: GLFWwindow, button: int, action: int, mods: int) -> None:
        app: App = glfwGetWindowUserPointer(window)

        if button == GLFW_MOUSE_BUTTON_LEFT:
            if action == GLFW_PRESS:
                app.mousePressed = True
                app.lastMouseLeftClickPos = copy.deepcopy(app.mousePos)
                app.lastMouseLeftPressPos = copy.deepcopy(app.mousePos)
                
                if app.debugMousePos:
                    print(f'mouseLeftPress @ {app.mousePos}')

            elif action == GLFW_RELEASE:
                app.mousePressed = False
                app.showPreview = True

                if app.debugMousePos:
                    print(f'mouseLeftRelease @ {app.mousePos}')
        
        elif button == GLFW_MOUSE_BUTTON_RIGHT:
            if action == GLFW_RELEASE:
                app.showPreview = False
    
    @staticmethod
    def __scrollCallback(window: GLFWwindow, xoffset: float, yoffset: float) -> None:
        pass
    
    @staticmethod
    def __perFrameTimeLogic(window: GLFWwindow) -> None:
        app: App = glfwGetWindowUserPointer(window);

        currentFrame: float = glfwGetTime();
        app.timeElapsedSinceLastFrame = currentFrame - app.lastFrameTimeStamp;
        app.lastFrameTimeStamp = currentFrame;
    
    @staticmethod
    def __processKeyInput(window: GLFWwindow) -> None:
        pass
        
    @staticmethod
    def __bresenhamLine(path: list[glm.float32], x0: int, y0: int, x1: int, y1: int) -> None:
        """
        Bresenham line-drawing algorithm for line (x0, y0) -> (x1, y1) in screen space,
        given that its slope m satisfies 0.0 <= m <= 1.0 and that (x0, y0) is the start position.
        All pixels on this line are appended to path 
        (a list of glm.float32s, each five glm.float32s constitute a pixel (x y) (r g b).)
        P.S. Returning a view of path is more Pythonic,
        however, we still modify the argument for consistency with the C++ version...
        """
        dx: int = abs(x1 - x0)
        dy: int = abs(y1 - y0)
        p: int = 2 * dy - dx
        twoDy: int = 2 * dy
        twoDyMinusDx: int = 2 * (dy - dx)

        x: int = x0
        y: int = y0

        path.append(x)
        path.append(y)
        path.append(1.0)
        path.append(1.0)
        path.append(1.0)

        while x < x1:
            x += 1

            if p < 0:
                p += twoDy
            else:
                y += 1
                p += twoDyMinusDx

            path.append(x)
            path.append(y)
            path.append(1.0)
            path.append(1.0)
            path.append(1.0)
        
    def __render(self) -> None:
        # Update all shader uniforms.
        self.pixelShader.use()
        self.pixelShader.setFloat("windowWidth", self.windowWidth)
        self.pixelShader.setFloat("windowHeight", self.windowHeight)

        # Render all shapes.
        for s in self.shapes:
            s.render()

