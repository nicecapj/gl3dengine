#pragma once
#include "Mesh.h"
#include "Renderer.h"

#include <vector>


class LitMeshRenderer : public Renderer
{
public:
    LitMeshRenderer(MeshType meshType, class Camera* camera, class LightRenderer* light);
    ~LitMeshRenderer();

	virtual void PreDraw() override;
	virtual void Draw() override;
	virtual void PostDraw() override;

private:
    class LightRenderer* light_ = nullptr;
    class Camera* camera_ = nullptr;

    std::vector<Vertex> vertices_;
    std::vector<GLuint> indicies_;

    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;

};

