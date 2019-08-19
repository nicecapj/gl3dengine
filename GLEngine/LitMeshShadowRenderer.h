#pragma once
#include "Mesh.h"
#include "Renderer.h"

#include <vector>


class LitMeshShadowRenderer : public Renderer
{
public:
	LitMeshShadowRenderer(MeshType meshType, class Camera* camera, class LightRenderer* light);
    ~LitMeshShadowRenderer();

    void Draw();


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

};

