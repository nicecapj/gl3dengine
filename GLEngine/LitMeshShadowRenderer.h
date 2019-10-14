#pragma once
#include "Mesh.h"
#include "Renderer.h"

#include <vector>

//static mesh의 경우, 라이트맵으로 그림자를 구워서, 합성하면 그만이다.
//그러나 움직이는 물체는 라이트맵으로 처리할 수 없다. 실시간 그림자는 매프레임마다 그림자를 생성해서
//합성해야 한다. ->shadowmap
class LitMeshShadowRenderer : public Renderer
{
public:
	LitMeshShadowRenderer(MeshType meshType, class Camera* camera, class LightRenderer* light);
    ~LitMeshShadowRenderer();

	virtual void PreDraw() override;
	virtual void Draw() override;
	virtual void PostDraw() override;

private:
    class LightRenderer* light_ = nullptr;
    class Camera* camera_ = nullptr;


	//최상위로 옮겨야 함
	GLuint depthMap_;
	GLuint depthMapFBO_;

	GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;	//아주 해상도가 큰걸		
	GLuint SCR_WIDTH = 1280, SCR_HEIGHT = 720;

	//-1~1을 ndc(0~1)로 바꾸기 위한 마스크, 성능에 좋지만, 교육용으로 쉐이더에서 하는것으로 변경
	glm::mat4 biasMatrix = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};

	glm::mat4 lightSpaceMatrix_;	//이건 engine에서 light를 관리하고, 거기서 매트릭스 얻어올수 있으면 된다. 일단 교육용으로 여기
	glm::mat4 lightProjection_;

};

