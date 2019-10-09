#pragma once
#include "Application.h"
class PostProcessginDemo :
	public Application
{
public:
	PostProcessginDemo();
	virtual ~PostProcessginDemo();

	virtual void Initialze() override;


	virtual void Finalize() override;


	virtual void UpdateScene(double deltaTimeMs) override;


	virtual void RenderScene() override;

protected:
	virtual void PostRenderScene() override;


	virtual void PreRenderScene() override;

private:
	GLuint sphereTexture = -1;
	
	class Model* meshModel_ = nullptr;
	GLuint modelShder_ = -1;
	GLuint screenShader_ = -1;

	class Camera* cam_ = nullptr;
	class LightRenderer* light_ = nullptr;
	class MeshRenderer* debugQuad = nullptr;

};

