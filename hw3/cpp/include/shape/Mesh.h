#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

#include "shape/GLShape.h"


class Shader;


/// Generic triangular mesh object.
class Mesh : public Renderable, public GLShape
{
public:
    struct Vertex
    {
        Vertex(const glm::vec3 & p, const glm::vec3 & n, const glm::vec3 & c) : position(p), normal(n), color(c) {}

        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 normal {0.0f, 0.0f, 0.0f};
        glm::vec3 color {1.0f, 1.0f, 1.0f};
    };

    Mesh(
        Shader * pShader,
        const std::vector<Vertex> & vertices,
        const glm::mat4 & model
    );

    ~Mesh() noexcept override = default;

    void render(float timeElapsedSinceLastFrame) override;

protected:
    // Used for children inheriting this class, e.g., Tetrahedron
    Mesh(Shader * shader, const glm::mat4 & model);

    std::vector<Vertex> vertices;
};


#endif  // MESH_H
