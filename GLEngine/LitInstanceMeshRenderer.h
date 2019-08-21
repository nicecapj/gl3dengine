#pragma once
#include "Mesh.h"
#include "ShaderLoader.h"
#include <vector>

#include "renderer.h"


//일반적인 메쉬 드로잉등, vao, vbo, ebo, 를 준비하고  DrawArray나 DrawElements를 매번 호출하여 gpu로 보냄으로,
//많은 오브젝트를 그릴때, 갯수 문제로 느려진다.
//1.하드웨어 인스턴싱은 opengl4.5, es3.0이상부티 지원
//2.소프트웨어 인스턴싱은 하드웨어 지원이 안될떄(es2.0이라던지), 버텍스쉐이더에 Constant에 카피한후, sm2.0일떄 256개, sm1.x일대 96개까지 가능
//3.constant인스턴싱 - 잘 모르겠다. 안 찾아보겠다.
//4. 스트림 인스턴싱 - 하드웨어 인스턴싱처럼 세컨드 버퍼 사용. 인스턴스 스트림버퍼의 오프셋 갱신해서 랜더링(gl_InstanceID와 비슷할듯)
//큰 변형이 없는 경우, 한번에 보내고, 그려버리면 빨라지는데, 이게 인스턴싱이다.
//glDrawArray->glDrawArraysInstanced
//glDrawElements-> glDrawElementsInstanced

//모두 동일한 객체임으로, 각 데이터 유지할 것 없이, gl_InstanceID라는 것을 추가했음
//버텍스배열에서, 어느 인덱스의 놈이야를 0번부터 가르킨다. 50번째 오브젝트는 gl_InstanceID == 49
class LitInstanceMeshRenderer : public Renderer
{
public:
    LitInstanceMeshRenderer(MeshType meshType, class Camera* camera, class LightRenderer* light);
    ~LitInstanceMeshRenderer();

    virtual void Draw() override;
    virtual void UpdateScene(double deltaTimeMs) override;

	void SetObjectCount(GLuint count);
	void SetTransforms(std::vector<glm::mat4>&& transforms);
private:
    class LightRenderer* light_ = nullptr;
    class Camera* camera_ = nullptr;    

	GLuint instancingBuffer_ = -1;
	GLuint objectCount_ = 1;
	std::vector<glm::mat4> transformList_;
};