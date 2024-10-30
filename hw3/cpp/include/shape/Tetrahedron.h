#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <string>

#include <glm/glm.hpp>

#include "shape/Mesh.h"


class Shader;


class Tetrahedron : public Mesh
{
public:
    Tetrahedron(Shader * pShader, const std::string & vertexFile, const glm::mat4 & model);

    ~Tetrahedron() noexcept override = default;

    void render(float timeElapsedSinceLastFrame) override;

private:
    static constexpr glm::vec3 kColor {0.31f, 0.5f, 1.0f};
};


#endif  // TETRAHEDRON_H
