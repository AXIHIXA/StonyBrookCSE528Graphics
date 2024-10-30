import copy
import ctypes

from OpenGL.GL import *
import glm

from .glshape import GLShape
from .renderable import Renderable
from util import Shader


class Line(GLShape, Renderable):
    def __init__(self, 
                 shader: Shader, 
                 vertices: glm.array, 
                 model: glm.mat4 = glm.mat4(1.0)):
        
        assert vertices.element_type == glm.float32 and vertices.length % 6 == 0, \
               'vertices should be alm.array of dtype glm.float32, ' \
               'each six glm.flost32s constitute a vertex (x, y, z, r, g, b)'
        
        super().__init__(shader, model)
        self.vertices: glm.array = copy.deepcopy(vertices)
        
        glBindVertexArray(self.vao)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)

        # Vertex coordinate attribute array "layout (position = 0) in vec3 aPosition"
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,                            # index: corresponds to "0" in "layout (position = 0)"
                              3,                            # size: each "vec3" generic vertex attribute has 3 values
                              GL_FLOAT,                     # data type: "vec3" generic vertex attributes are GL_FLOAT
                              GL_FALSE,                     # do not normalize data
                              6 * glm.sizeof(glm.float32),  # stride between attributes in VBO data
                              None)                         # offset of 1st attribute in VBO data

        # Vertex color attribute array "layout (position = 1) in vec3 aColor"
        glEnableVertexAttribArray(1)
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              6 * glm.sizeof(glm.float32),
                              ctypes.c_void_p(3 * glm.sizeof(glm.float32)))

        glBufferData(GL_ARRAY_BUFFER,
                     self.vertices.nbytes,
                     self.vertices.ptr,
                     GL_STATIC_DRAW)

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)
    
    def render(self, timeElapsedSinceLastFrame: int) -> None:
        self.shader.use()
        self.shader.setMat4("model", self.model)

        glBindVertexArray(self.vao)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)

        glDrawArrays(GL_LINES,
                     0,                          # start from index 0 in current VBO
                     self.vertices.length // 6)  # draw these number of vertice attributes

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)

