import copy
import ctypes

from OpenGL.GL import *
import glm

from .glshape import GLShape
from .renderable import Renderable
from util import Shader


class Circle(GLShape, Renderable):
    def __init__(self, 
                 shader: Shader, 
                 parameters: glm.array):
                 
        assert parameters.element_type == glm.float32 and parameters.length % 3 == 0, \
               'parameters should be alm.array of dtype glm.float32, ' \
               'each three glm.flost32s constitute a circle (x, y, r)'
              
        super().__init__(shader)
        self.parameters: glm.array = copy.deepcopy(parameters)
        
        glBindVertexArray(self.vao)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)
        
        # Vertex coordinate attribute array "layout (position = 0) in vec3 aPos"
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,                            # index: corresponds to "0" in "layout (position = 0)"
                              3,                            # size: each "vec3" generic vertex attribute has 3 values
                              GL_FLOAT,                     # data type: "vec3" generic vertex attributes are GL_FLOAT
                              GL_FALSE,                     # do not normalize data
                              3 * glm.sizeof(glm.float32),  # stride between attributes in VBO data
                              None)                         # offset of 1st attribute in VBO data

        glBufferData(GL_ARRAY_BUFFER,
                     self.parameters.nbytes,
                     self.parameters.ptr,
                     GL_STATIC_DRAW)
        
        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)
        
    def render(self, timeElapsedSinceLastFrame: int, animate: bool) -> None:
        if animate:
            self.model = glm.rotate(self.model, timeElapsedSinceLastFrame)
        
        self.shader.use()
        self.shader.setMat3("model", self.model)

        glBindVertexArray(self.vao)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)

        glPatchParameteri(GL_PATCH_VERTICES, 1)
        glDrawArrays(GL_PATCHES,
                     0,                       # start from index 0 in current VBO
                     self.parameters.length)  # draw these number of elements

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)
 
