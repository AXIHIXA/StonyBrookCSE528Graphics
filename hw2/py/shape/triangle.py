import copy
import ctypes

from OpenGL.GL import *
import glm

from .glshape import GLShape
from .renderable import Renderable
from util import Shader


class Triangle(GLShape, Renderable):
    def __init__(self, 
                 shader: Shader, 
                 vertices: glm.array, 
                 model: glm.mat3 = glm.mat3(1.0)):
        
        assert vertices.element_type == glm.float32 and vertices.length % 5 == 0, \
               'vertices should be alm.array of dtype glm.float32, ' \
               'each five glm.flost32s constitute a vertex (x, y, r, g, b)'
        
        super().__init__(shader, model)
        self.vertices: glm.array = copy.deepcopy(vertices)
        
        glBindVertexArray(self.vao);
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo);

        # Vertex position attribute array "layout (position = 0) in vec2 aPosition"
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,                            # index: corresponds to "0" in "layout (position = 0)"
                              2,                            # size: each "vec2" generic vertex attribute has 2 values
                              GL_FLOAT,                     # data type: "vec2" generic vertex attributes are GL_FLOAT
                              GL_FALSE,                     # do not normalize data
                              5 * glm.sizeof(glm.float32),  # stride between attributes in VBO data
                              None)                         # offset of 1st attribute in VBO data

        # Vertex color attribute array "layout (position = 1) in vec3 aColor"
        glEnableVertexAttribArray(1)
        glVertexAttribPointer(1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              5 * glm.sizeof(glm.float32),
                              ctypes.c_void_p(2 * glm.sizeof(glm.float32)))

        glBufferData(GL_ARRAY_BUFFER,
                     self.vertices.nbytes,
                     self.vertices.ptr,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
    def render(self, timeElapsedSinceLastFrame: int, animate: bool) -> None:
        if animate:
            self.model = glm.rotate(self.model, timeElapsedSinceLastFrame)

        self.shader.use()
        self.shader.setMat3("model", self.model)

        glBindVertexArray(self.vao)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)

        glDrawArrays(GL_TRIANGLES,
                     0,                     # start from index 0 in current VBO
                     3)                     # draw these number of vertices

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)

