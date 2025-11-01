#include <glm/glm.hpp>

#include "shape/Tetrahedron.h"
#include "util/Shader.h"


Tetrahedron::Tetrahedron(
        Shader * pShader,
        const std::string & vertexFile,
        const glm::mat4 & model
)
        : Mesh(pShader, model)
{
    // Initialize vertex data
    if (std::ifstream fin {vertexFile})
    {
        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 v3;

        while (fin >> v1.x >> v1.y >> v1.z >> v2.x >> v2.y >> v2.z >> v3.x >> v3.y >> v3.z)
        {
            glm::vec3 fn = glm::normalize(glm::cross(v2 - v1, v3 - v2));
            vertices.emplace_back(v1, fn, kColor);
            vertices.emplace_back(v2, fn, kColor);
            vertices.emplace_back(v3, fn, kColor);
        }
    }
    else
    {
        throw std::runtime_error("failed to open " + vertexFile);
    }

    // OpenGL pipeline configuration
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(this->vertices.size() * sizeof(Vertex)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Tetrahedron::render(float timeElapsedSinceLastFrame)
{
    pShader->use();
    pShader->setMat4("model", model);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDrawArrays(GL_TRIANGLES,
                 0,                                       // start from index 0 in current VBO
                 static_cast<GLsizei>(vertices.size()));  // draw these number of elements

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}