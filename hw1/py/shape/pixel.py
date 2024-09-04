import copy
import ctypes

from OpenGL.GL import *
import glm

from .glshape import GLShape
from .renderable import Renderable
from util import Shader


class Pixel(GLShape, Renderable):
    def __init__(self, 
                 shader: Shader):
        
        super().__init__(shader, glm.mat3(1.0))
        
        # path:     List of pixels-to-draw, each pixel constitutes of five glm.float32s: (x y r g b);
        # dirty:    Whether the list of path is modified (needs to be flushed into OpenGL buffer);
        # vertices: PyGLM array holding path, OpenGL APIs extract contents from self.vertices.
        self.dirty: bool = False
        self.path: list[glm.float32] = []
        self.vertices: glm.array = glm.array(glm.float32, 0.0)
        
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

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)
    
    def render(self) -> None:
        self.shader.use()

        glBindVertexArray(self.vao)
        glBindBuffer(GL_ARRAY_BUFFER, self.vbo)
        
        if self.dirty:
            assert len(self.path) % 5 == 0, \
                   'Pixel.path should be a list of glm.float32s, ' \
                   'each five glm.flost32s constitute a pixel (x y) (r g b)'
            
            self.vertices = glm.array(glm.float32, *self.path)
        
            glBufferData(GL_ARRAY_BUFFER,
                         self.vertices.nbytes,
                         self.vertices.ptr,
                         GL_DYNAMIC_DRAW)
            
            self.dirty = False

        glDrawArrays(GL_POINTS,
                     0,                          # start from index 0 in current VBO
                     self.vertices.length // 5)  # draw these number of vertices

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindVertexArray(0)

