#pragma once

#include <vector>
#include "Renderer.h"
class CubemanRenderer : public Renderer
{
public:
	CubemanRenderer(GLuint shader, class Camera* camera, class LightRenderer* light);
	~CubemanRenderer();
	
	virtual void PreDraw() override;
	virtual void Draw() override;
	virtual void PostDraw() override;	
	virtual void UpdateScene(double deltaTimeMs) override;


	//�����
	virtual void SetProgram(GLuint program) override;
	virtual void SetTexture(int index, GLuint textureID) override;
	virtual void SetEnableDynamicShadow(bool isEnable) override;

public:
	class LightRenderer* light_ = nullptr;
	class Camera* camera_ = nullptr;
	
	Renderer* root_;
};
