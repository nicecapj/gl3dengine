#pragma once
#include "Renderer.h"
#include "mesh.h"
#include <string>

class SkyBoxRenderer : public Renderer
{
public:
	SkyBoxRenderer(MeshType meshType, class Camera* camera);
	~SkyBoxRenderer();

	//load cubemap texture
	//cubemapName_left / cubemapName_right
	//cubemapName_front / cubemapName_back
	//cubemapName_top / cubemapName_bottom
	void GenerateCubemap(std::string cubemapName);

	virtual void PreDraw() override;
	virtual void Draw() override;
	virtual void PostDraw() override;

private:
	Camera* camera_;

};

