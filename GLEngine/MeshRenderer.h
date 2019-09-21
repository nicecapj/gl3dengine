#pragma once

#include <gl/glew.h>

#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"

#include <vector>

class MeshRenderer : public Renderer
{
public:
    MeshRenderer(MeshType meshType, Camera* camera);
    ~MeshRenderer();

    virtual void Draw() override;	
	virtual void PreDraw() override;
	virtual void PostDraw() override;


	virtual void UpdateScene(glm::mat4* matParentModel, double deltaTimeMs) override;

private:
    Camera* camera_;

	
};

