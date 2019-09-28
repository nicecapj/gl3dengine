#include "pch.h"
#include "GLEngine.h"

#include <iostream>


GLEngine::GLEngine()
{}

GLEngine::~GLEngine()
{
	Destroy();
}

void GLEngine::InitPhysics()
{
	//물리지원 아직 안함.
}

//쉐도우매핑에 필요한 버퍼 및 텍스처 생성
void GLEngine::InitShadowmap()
{
	glGenFramebuffers(1, &depthMapFBO_);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);

	//깊이 텍스쳐는 깊이버퍼보다 느리지만, 쉐이더에서 텍스처 샘플로 사용할수 있는 장점이 있다.	
	//unsigned int depthMap;
	glGenTextures(1, &depthMap_);
	glBindTexture(GL_TEXTURE_2D, depthMap_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//GL_DEPTH_COMPONENT16 / 24/ 32가 있다. 24비트 정밀도가 일반적으로 적당
	const int MIPMAP_LEVEL = 0;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap_, MIPMAP_LEVEL);

	glDrawBuffer(GL_NONE);	//버퍼에 컬러 그리지 않음
	glReadBuffer(GL_NONE);	//버퍼에서 컬러 읽지 않음.


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "error framebuffer" << std::endl;
	}
}

//포스트포르세싱에 필요한 랜더텍스쳐 및 버퍼 생성
void GLEngine::InitPostProcessing()
{
	//RENDER
	const int MIPMAP_LEVEL = 0;
	glGenFramebuffers(1, &postprocessFBO_);
	glBindFramebuffer(GL_FRAMEBUFFER, postprocessFBO_);
	glGenTextures(1, &postprocessMap_);
	glBindTexture(GL_TEXTURE_2D, postprocessMap_);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postprocessMap_, MIPMAP_LEVEL);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "error post processing render buffer" << std::endl;
	}


	//RENDER BUFFER객체 생성, TEXTURE에 비해 빠르나, READ가 안됨. TEXTURE방식 다음에 나온것으로 성능에 좋다.	
	glGenRenderbuffers(1, &rbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GLEngine::GetEnvrionmentMap()
{
	return envMap_;
}

void GLEngine::SetEnvrionmentMap(GLuint textureId)
{
	envMap_ = textureId;
}

GLuint GLEngine::GetShadowmapWidth()
{
	return SHADOW_WIDTH;
}

GLuint GLEngine::GetShadowmapHeight()
{
	return SHADOW_HEIGHT;
}

GLuint GLEngine::GetSceneTexture()
{
	return postprocessMap_;
}

GLuint GLEngine::GetSceneBuffer()
{
	return postprocessFBO_;
}

void GLEngine::Destroy()
{
	glDeleteBuffers(1, &depthMapFBO_);
	glDeleteBuffers(1, &postprocessFBO_);
}

