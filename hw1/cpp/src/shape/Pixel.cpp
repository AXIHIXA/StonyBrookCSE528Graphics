#include "shape/Pixel.h"
#include "util/Shader.h"


Pixel::Vertex::Vertex(int x, int y, float r, float g, float b): 
    position(x, y), color(r, g, b)
{
    
}


Pixel::Pixel(Shader * shader) : GLShape(shader)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Vertex coordinate attribute array "layout (position = 0) in vec2 aPosition"
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                             // index: corresponds to "0" in "layout (position = 0)"
                          2,                             // size: each "vec2" generic vertex attribute has 2 values
                          GL_FLOAT,                      // data type: "vec2" generic vertex attributes are GL_FLOAT
                          GL_FALSE,                      // do not normalize data
                          sizeof(Vertex),                // stride between attributes in VBO data
                          reinterpret_cast<void *>(0));  // offset of 1st attribute in VBO data

    // Color vertex attribute array "layout (position = 1) in vec3 aColor"
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void *>(sizeof(Vertex::position)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Pixel::render()
{
    pShader->use();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (dirty)
    {
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(path.size() * sizeof(Vertex)),
                     path.data(),
                     GL_DYNAMIC_DRAW);

        dirty = false;
    }

    glDrawArrays(GL_POINTS,
                 0,                                       // start from index 0 in current VBO
                 static_cast<GLsizei>(path.size()));  // draw these number of elements

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
