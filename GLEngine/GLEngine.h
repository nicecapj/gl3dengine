#pragma once
#include "Singleton.h"
#include <gl/glew.h>

#define g_GLEngine GLEngine::GetInstance()

class GLEngine : public Singleton< GLEngine>
{
public:
	GLEngine();
	~GLEngine();

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const GLuint SCR_WIDTH = 1280, SCR_HEIGHT = 720;

	void InitPhysics();	

	void InitShadowmap();
	void InitPostProcessing();

	GLuint GetScreenSizeX() { return SCR_WIDTH; }
	GLuint GetScreenSizeY() { return SCR_HEIGHT; }
	
	GLuint GetEnvrionmentMap();
	void SetEnvrionmentMap(GLuint textureId);

	GLuint GetShadowmapBuffer() { return depthMapFBO_; }
	GLuint GetShadowmap() { return depthMap_; }
	GLuint GetShadowmapWidth();
	GLuint GetShadowmapHeight();

	void Destroy();

private:
	GLuint envMap_ = -1;
	GLuint depthMapFBO_;
	GLuint depthMap_;

	GLuint postprocessFBO_;
	GLuint postprocessMap_;
	GLuint rbo_;
};