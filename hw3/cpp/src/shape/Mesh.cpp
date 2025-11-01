#include "shape/Mesh.h"
#include "util/Shader.h"


Mesh::Mesh(
        Shader * shader,
        const std::vector<Vertex> & vertices,
        const glm::mat4 & model
)
        : Mesh(shader, model)
{
    this->vertices = vertices;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(vertices.size() * sizeof(Vertex)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0U);
}


void Mesh::render(float timeElapsedSinceLastFrame)
{
    pShader->use();
    pShader->setMat4("model", model);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDrawArrays(GL_TRIANGLES,
                 0,                                       // start from index 0 in current VBO
                 static_cast<GLsizei>(vertices.size()));  // draw these number of elements

    glBindBuffer(GL_ARRAY_BUFFER, 0U);
    glBindVertexArray(0U);
}


Mesh::Mesh(Shader * shader, const glm::mat4 & model) : GLShape(shader, model)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Vertex coordinate attribute array "layout (position = 0) in vec3 aPosition"
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                             // index: corresponds to "0" in "layout (position = 0)"
                          3,                             // size: each "vec3" generic vertex attribute has 3 values
                          GL_FLOAT,                      // data type: "vec3" generic vertex attributes are GL_FLOAT
                          GL_FALSE,                      // do not normalize data
                          sizeof(Vertex),                // stride between attributes in VBO data
                          reinterpret_cast<void *>(0));  // offset of 1st attribute in VBO data

    // Normal vertex attribute array "layout (position = 1) in vec3 aNormal"
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void *>(sizeof(Vertex::position)));

    // Color vertex attribute array "layout (position = 2) in vec3 aColor"
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void *>(sizeof(Vertex::position) + sizeof(Vertex::normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
