#pragma once

#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "Renderer.h"

class LightRenderer : public Renderer
{
public:
    LightRenderer(MeshType meshType, Camera* camera);
    ~LightRenderer();

    virtual void Draw() override;

private:
    Camera* camera_;
};

