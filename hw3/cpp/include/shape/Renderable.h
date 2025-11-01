/// STOP. You should not modify this file unless you KNOW what you are doing.

#ifndef RENDERABLE_H
#define RENDERABLE_H


/// Abstract class (interface) representing an object-to-render.
/// All shapes should public-inherit this class.
/// Note that for polymorphism usage, we must go public inheritance.
/// Conversion from pointer-to-derived to pointer-to-base is prohibited
/// for private and protected inheritance.
class Renderable
{
public:
    virtual ~Renderable() noexcept = 0;

    virtual void render(float timeElapsedSinceLastFrame) = 0;
};



#endif  // RENDERABLE_H
