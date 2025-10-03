#ifndef CIRCLE_H
#define CIRCLE_H


#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shape/GLShape.h"


class Shader;


// Circle[s] class, this class represents MULTIPLE circles.
class Circle : public Renderable, public GLShape
{
public:
    Circle(Shader * shader, const std::vector<glm::vec3> & parameters);

    ~Circle() noexcept override = default;

    void render(float timeElapsedSinceLastFrame, bool animate) override;

private:
    std::vector<glm::vec3> parameters;
};


#endif  // CIRCLE_H
