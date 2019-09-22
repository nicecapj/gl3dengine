#pragma once

#include <vector>
#include "Renderer.h"
class CubemanRenderer : public Renderer
{
public:
	CubemanRenderer(class Camera* camera, class LightRenderer* light);
	~CubemanRenderer();
	
	virtual void PreDraw() override;
	virtual void Draw() override;
	virtual void PostDraw() override;	


	//¾î°ÅÁö	
	virtual void SetEnableDynamicShadow(bool isEnable) override;

public:
	class LightRenderer* light_ = nullptr;
	class Camera* camera_ = nullptr;		
};

