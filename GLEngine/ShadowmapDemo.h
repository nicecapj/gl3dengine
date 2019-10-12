#pragma once
#include "Application.h"
#include "LitMeshShadowRenderer.h"

class ShadowmapDemo :
	public Application
{
public:
	ShadowmapDemo();
	~ShadowmapDemo();

	virtual void Initialze() override;
	virtual void Finalize() override;
	virtual void UpdateScene(double deltaTimeMs) override;
	virtual void RenderScene() override;


	virtual class Camera* GetCamera() override;

protected:
	virtual void PostRenderScene() override;
	virtual void PreRenderScene() override;

private:
	std::vector<Renderer*> shadowRenderList_;
	GLuint sphereTexture = -1;

	class Camera* cam_ = nullptr;
	class LightRenderer* light_ = nullptr;
	class MeshRenderer* debugQuad = nullptr;

};

