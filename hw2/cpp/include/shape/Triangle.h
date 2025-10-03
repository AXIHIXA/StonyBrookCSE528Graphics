#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shape/GLShape.h"


class Shader;


class Triangle : public Renderable, public GLShape
{
public:
    struct Vertex
    {
        glm::vec2 position;
        glm::vec3 color;
    };

    explicit Triangle(
            Shader * shader,
            const std::vector<Vertex> & vertices,
            const glm::mat3 & model = glm::mat3(1.0f)
    );

    ~Triangle() noexcept override = default;

    void render(float timeElapsedSinceLastFrame, bool animate) override;

private:
    std::vector<Vertex> vertices;
};


#endif  // TRIANGLE_H
