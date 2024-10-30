#ifndef LINE_H
#define LINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shape/GLShape.h"


class Shader;


class Line : public Renderable, public GLShape
{
public:
    struct Vertex
    {
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 color {1.0f, 1.0f, 1.0f};
    };

    Line(Shader * pShader, const std::vector<Vertex> & vertices, const glm::mat4 & model);

    ~Line() noexcept override = default;

    void render(float timeElapsedSinceLastFrame) override;

private:
    std::vector<Vertex> vertices;
};


#endif  // LINE_H
