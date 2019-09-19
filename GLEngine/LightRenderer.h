#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"

class LightRenderer : public Renderer
{
public:
    LightRenderer(MeshType meshType, Camera* camera);
    ~LightRenderer();
	
	virtual void Draw() override;	


	virtual void PreDraw() override;


	virtual void PostDraw() override;

private:
    Camera* camera_;
};

