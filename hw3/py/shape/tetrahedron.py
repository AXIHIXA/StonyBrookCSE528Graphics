import ctypes

from OpenGL.GL import *
import glm

from .mesh import Mesh
from util import Shader


class Tetrahedron(Mesh):
    color: glm.vec3 = glm.vec3(0.31, 0.5, 1.0)
    
    def __init__(self, 
                 shader: Shader, 
                 vertexFile: str, 
                 model: glm.mat4 = glm.mat4(1.0)):

        floatList: list[float] = []
        
        with open(vertexFile, 'r') as fin:
            floatList.extend(map(lambda x: float(x), fin.read().split()))
            
        assert len(floatList) % 9 == 0, \
               'vertexFile should contain 9n floats, ' \
               'each three floats denote a vertex (x, y, z), ' \
               'each three vertices denote a triangular facet'
        
        vertexList: list[float] = []
        
        for i in range(0, len(floatList), 9):
            v1: glm.vec3 = glm.vec3(floatList[i],     floatList[i + 1], floatList[i + 2])
            v2: glm.vec3 = glm.vec3(floatList[i + 3], floatList[i + 4], floatList[i + 5])
            v3: glm.vec3 = glm.vec3(floatList[i + 6], floatList[i + 7], floatList[i + 8])
            faceNormal: glm.vec3 = glm.normalize(glm.cross(v2 - v1, v3 - v2))
            
            vertexList.append(v1.x)
            vertexList.append(v1.y)
            vertexList.append(v1.z)
            vertexList.append(faceNormal.x)
            vertexList.append(faceNormal.y)
            vertexList.append(faceNormal.z)
            vertexList.append(self.color.x)
            vertexList.append(self.color.y)
            vertexList.append(self.color.z)
            
            vertexList.append(v2.x)
            vertexList.append(v2.y)
            vertexList.append(v2.z)
            vertexList.append(faceNormal.x)
            vertexList.append(faceNormal.y)
            vertexList.append(faceNormal.z)
            vertexList.append(self.color.x)
            vertexList.append(self.color.y)
            vertexList.append(self.color.z)
            
            vertexList.append(v3.x)
            vertexList.append(v3.y)
            vertexList.append(v3.z)
            vertexList.append(faceNormal.x)
            vertexList.append(faceNormal.y)
            vertexList.append(faceNormal.z)
            vertexList.append(self.color.x)
            vertexList.append(self.color.y)
            vertexList.append(self.color.z)
        
        self.vertices: glm.array = glm.array(glm.float32, *vertexList)
        
        # Python does not support delegate constructors, 
        # implementing a classmethod called "fromFile" is much more Pythonic.
        # Yet, we invoke Mesh.__init__ for consistency with the C++ version...
        super().__init__(shader, self.vertices, model)
    
    # Leave "render" method as-is as imlemented in class Mesh...

