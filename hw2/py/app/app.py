import copy

from OpenGL.GL import *
from glfw.GLFW import *
from glfw import _GLFWwindow as GLFWwindow
import glm

from .window import Window
from shape import Circle, Renderable, Triangle
from util import Shader


class App(Window):
    def __init__(self):
        self.windowName: str = 'hw2'
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
        self.triangleShader: Shader = \
            Shader(vert='shader/triangle.vert.glsl', 
                   tesc=None,
                   tese=None,
                   frag='shader/triangle.frag.glsl')
                   
        self.circleShader: Shader = \
            Shader(vert='shader/circle.vert.glsl', 
                   tesc='shader/circle.tesc.glsl',
                   tese='shader/circle.tese.glsl',
                   frag='shader/circle.frag.glsl')
        
        # Objects to render.
        self.shapes: list[Renderable] = []
        
        self.shapes.append(
            Triangle(
                self.triangleShader, 
                glm.array(
                    # dtype
                    glm.float32,
                    # positions    # colors
                    200.0, 326.8,  1.0, 0.0, 0.0,  # bottom right
                    800.0, 326.8,  0.0, 1.0, 0.0,  # bottom left
                    500.0, 846.4,  0.0, 0.0, 1.0   # top
                )
            )
        )
        
        self.shapes.append(
            Circle(
                self.circleShader, 
                glm.array(
                    # dtype
                    glm.float32,
                    # center pos   # radius
                    200.0, 326.8,  200.0,
                    800.0, 326.8,  300.0,
                    500.0, 846.4,  400.0
                )
            )
        )

        # Object attributes affected by GUI.
        self.animationEnabled: bool = True

        # Frontend GUI
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
    
    @staticmethod
    def __framebufferSizeCallback(window: GLFWwindow, width: int, height: int) -> None:
        glViewport(0, 0, width, height)
    
    @staticmethod
    def __keyCallback(window: GLFWwindow, key: int, scancode: int, action: int, mods: int) -> None:
        app: App = glfwGetWindowUserPointer(window)

        if key == GLFW_KEY_A and action == GLFW_RELEASE:
            app.animationEnabled = not app.animationEnabled
        
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

                if app.debugMousePos:
                    print(f'mouseLeftRelease @ {app.mousePos}')
    
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
    
    def __render(self) -> None:
        t: float = self.timeElapsedSinceLastFrame

        # Update all shader uniforms.
        self.triangleShader.use()
        self.triangleShader.setFloat("windowWidth", self.windowWidth)
        self.triangleShader.setFloat("windowHeight", self.windowHeight)
        
        self.circleShader.use()
        self.circleShader.setFloat("windowWidth", self.windowWidth)
        self.circleShader.setFloat("windowHeight", self.windowHeight)

        # Render all shapes.
        for s in self.shapes:
            s.render(t, self.animationEnabled)

