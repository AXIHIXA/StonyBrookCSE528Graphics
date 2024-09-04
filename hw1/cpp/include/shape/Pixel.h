#ifndef PIXEL_H
#define PIXEL_H

#include <functional>
#include <mutex>

#include "shape/GLShape.h"


class Pixel : public Renderable, public GLShape
{
public:
    struct Vertex
    {
        Vertex() = default;
        Vertex(int, int, float, float, float);
        
        glm::vec2 position;
        glm::vec3 color;
    };

    explicit Pixel(Shader * shader);

    ~Pixel() noexcept override = default;

    void render() override;

    // `path` stores all pixels (in screen-space coodinates) to draw.
    // `dirty` should be set to true when path is updated
    // (otherwise the update won't happen on the screen.)
    bool dirty {false};
    std::vector<Vertex> path;
};


#endif  // PIXEL_H
