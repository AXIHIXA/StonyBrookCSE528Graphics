/// STOP. You should not modify this file unless you KNOW what you are doing.

#include "shape/GLShape.h"


GLShape::~GLShape() noexcept
{
    glDeleteVertexArrays(1, &vao);
    vao = 0U;

    glDeleteBuffers(1, &vbo);
    vbo = 0U;
}


GLShape::GLShape(Shader * pShader, const glm::mat4 & model) : pShader(pShader), model(model)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}


GLShape::GLShape(GLShape && rhs) noexcept
{
    *this = std::move(rhs);
}


GLShape & GLShape::operator=(GLShape && rhs) noexcept
{
    if (this == &rhs)
    {
        return *this;
    }

    pShader = rhs.pShader;
    rhs.pShader = nullptr;

    vao = rhs.vao;
    rhs.vao = 0U;

    vbo = rhs.vbo;
    rhs.vbo = 0U;

    model = rhs.model;

    return *this;
}
