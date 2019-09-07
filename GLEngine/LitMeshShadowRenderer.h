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

    std::vector<Vertex> vertices_;
    std::vector<GLuint> indicies_;

	//최상위로 옮겨야 함
	GLuint depthMap_;
	GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;	//아주 해상도가 큰걸	
	GLuint depthMapFBO_;
	GLuint SCR_WIDTH = 1280, SCR_HEIGHT = 720;

    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

	//-1~1을 0~1로 바꾸기 위한 마스크
	glm::mat4 biasMatrix = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};

};

