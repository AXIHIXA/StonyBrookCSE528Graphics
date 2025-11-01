"""
STOP. You should not modify this file unless you KNOW what you are doing.
"""

import copy

from OpenGL.GL import *
import glm

from util import Shader



class GLShape: 
    """
    Generic Shape object that manages the OpenGL context for a shape.
    All shapes directly interacting with the OpenGL context
    should public-inherit this class.
    """
    def __init__(self, 
                 shader: Shader, 
                 model: glm.mat4 = glm.mat4(1.0)):
        self.shader: Shader = shader
        self.vao: int = glGenVertexArrays(1)
        self.vbo: int = glGenBuffers(1)
        self.model: glm.mat4 = copy.deepcopy(model)
    
    def __del__(self):
        glDeleteVertexArrays(1, (self.vao,))
        self.vao = 0
        glDeleteBuffers(1, (self.vbo,))
        self.vbo = 0

