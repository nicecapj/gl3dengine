#pragma once

#include <gl/glew.h>

#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "Renderer.h"

#include <vector>

class MeshRenderer : public Renderer
{
public:
    MeshRenderer(MeshType meshType, Camera* camera);
    ~MeshRenderer();

    virtual void Draw() override;

private:
    Camera* camera_;
};

