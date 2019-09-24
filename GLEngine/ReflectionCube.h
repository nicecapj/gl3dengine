#pragma once
#include "Renderer.h"
class ReflectionCube :
    public Renderer
{
public:
    ReflectionCube(MeshType meshType, class Camera* camera);
    ~ReflectionCube();

    virtual void PreDraw() override;
    virtual void Draw() override;
    virtual void PostDraw() override;

private:
    Camera* camera_;

};

