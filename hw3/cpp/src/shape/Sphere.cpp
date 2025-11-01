#include "shape/Sphere.h"
#include "util/Shader.h"


Sphere::Sphere(
        Shader * pShader,
        const glm::vec3 & center,
        float radius,
        const glm::vec3 & color,
        const glm::mat4 & model
)
        : GLShape(pShader, model),
          center(center),
          radius(radius),
          color(color)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Placeholder attribute array "layout (position = 0) in float null"
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                             // index: corresponds to "0" in "layout (position = 0)"
                          1,                             // size: each "vec3" generic vertex attribute has 3 values
                          GL_FLOAT,                      // data type: "vec3" generic vertex attributes are GL_FLOAT
                          GL_FALSE,                      // do not normalize data
                          sizeof(float),                // stride between attributes in VBO data
                          reinterpret_cast<void *>(0));  // offset of 1st attribute in VBO data

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(sizeof(float)),
                 &kNull,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Sphere::render(float timeElapsedSinceLastFrame)
{
    pShader->use();
    pShader->setMat4("model", model);
    pShader->setVec3("center", center);
    pShader->setFloat("radius", radius);
    pShader->setVec3("color", color);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glPatchParameteri(GL_PATCH_VERTICES, 1);
    glDrawArrays(GL_PATCHES, 0, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}