#pragma once
#include "Application.h"
class DynamicFontDemo :
	public Application
{
public:
	DynamicFontDemo();
	virtual ~DynamicFontDemo();

	virtual void Initialze() override;
	virtual void Finalize() override;
	virtual void UpdateScene(double deltaTimeMs) override;
	virtual void RenderScene() override;
	virtual void ProcessKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) override;

protected:
	virtual void PostRenderScene() override;
	virtual void PreRenderScene() override;

private:
	class TextRenderer* label_ = nullptr;
};

