#include "pch.h"
#include "LitMeshShadowRenderer.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "LightRenderer.h"


LitMeshShadowRenderer::LitMeshShadowRenderer(MeshType meshType, Camera* camera, class LightRenderer* light)
    :Renderer()
{
    light_ = light;
    camera_ = camera;

    switch (meshType)
    {
        case MeshType::Trangile:
            {
                Mesh::setTriData(vertices_, indicies_);
            } break;
        case MeshType::Cube:
            {
                Mesh::setCubeData(vertices_, indicies_);
            } break;
        case MeshType::Sphere:
            {
                Mesh::setSphereData(vertices_, indicies_);
            } break;
        case MeshType::Quad:
            {
                Mesh::setQuadData(vertices_, indicies_);
            } break;
    }

    //VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    //VBO
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);	//STATIC


    glEnableVertexAttribArray(0);	//VS : layout (location = 0) in vec3 position;
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)0);

    glEnableVertexAttribArray(1);	//layout (location = 1) in vec2 texCoord;
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)(offsetof(Vertex, Vertex::texCoords)));

    glEnableVertexAttribArray(2);	//VS : layout (location = 2) in vec3 normal;
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (GLvoid*)(offsetof(Vertex, Vertex::normal)));

    //glEnableVertexAttribArray(3);	//VS : layout (location = 3) in vec3 color;
    //glVertexAttribPointer(3,
    //                      3,
    //                      GL_FLOAT,
    //                      GL_FALSE,
    //                      sizeof(Vertex),
    //                      (GLvoid*)(offsetof(Vertex, Vertex::color)));

    //EBO
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indicies_.size(), &indicies_[0], GL_STATIC_DRAW);

    //Unbind buffer and vertex array as a precaution
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


LitMeshShadowRenderer::~LitMeshShadowRenderer()
{
}

void LitMeshShadowRenderer::Draw()
{	   

}

void LitMeshShadowRenderer::PreDraw()
{
	if (enableDynamicShadow_)
	{
		glUseProgram(program_);

		glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);
		glm::mat4 model = transformMatrix * scaleMatrix;


		//projection from light
		//Light space transform
		float near_plane = -100.0f, far_plane = 100.5f;
		glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
		//glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 InvlightPos(-light_->GetPosition().x, light_->GetPosition().y, -light_->GetPosition().z);
		glm::mat4 lightView = glm::lookAt(InvlightPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		glm::mat4 depthBiasMVP = lightSpaceMatrix * biasMatrix;

		GLuint lightSpaceMatrixLocation = glGetUniformLocation(program_, "lightSpaceMatrix");
		glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

		/*GLuint DepthBiasMVPLocation = glGetUniformLocation(program_, "DepthBiasMVP");
		glUniformMatrix4fv(DepthBiasMVPLocation, 1, GL_FALSE, glm::value_ptr(depthBiasMVP));

		*/

		GLuint modelLocation = glGetUniformLocation(program_, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		//draw
		//한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, indicies_.size(), GL_UNSIGNED_INT, 0);

		//marks end of draw function
		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void LitMeshShadowRenderer::PostDraw()
{
	glUseProgram(program_);

	//MVP
	glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0), position_);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale_);
	glm::mat4 model = transformMatrix * scaleMatrix;

	glm::mat4 view = camera_->GetViewMatrix();
	glm::mat4 proj = camera_->GetProjectMatrix();

	GLuint modelLocation = glGetUniformLocation(program_, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	GLuint vpLocation = glGetUniformLocation(program_, "view");
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionpLocation = glGetUniformLocation(program_, "projection");
	glUniformMatrix4fv(projectionpLocation, 1, GL_FALSE, glm::value_ptr(proj));


	//projection from light
	//Light space transform
	//빛에서 평행하게 광선이 진행해야 하는데, 원근이 적용되면 변형됨으로, 직교투영으로 한다.
	//원하는 오브젝트가 그림자를 뎁스맵에 그리려 해도, 클리핑되면 안됨으로. near / far_plane을 잘 지정해야 한다.
	float near_plane = -100.0f, far_plane = 100.5f;
	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
	//glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 InvlightPos(-light_->GetPosition().x, light_->GetPosition().y, -light_->GetPosition().z);
	glm::mat4 lightView = glm::lookAt(InvlightPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	glm::mat4 depthBiasMVP = lightSpaceMatrix * biasMatrix;

	GLuint lightSpaceMatrixLocation = glGetUniformLocation(program_, "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	auto camPos = camera_->GetCameraPosition();
	GLuint camPosLocation = glGetUniformLocation(program_, "viewPos");	//uniform vec3 cameraPos;
	assert(camPosLocation != -1);
	glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);

	GLuint lightPosLocation = glGetUniformLocation(program_, "lightPos");	//uniform vec3 lightPos;
	//assert(lightPosLocation != -1);
	auto lightPos = light_->GetPosition();
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

	//multi texture			
	GLuint diffuseTextureLocation = glGetUniformLocation(program_, "diffuseTexture");
	glUniform1i(diffuseTextureLocation, 0);
	glActiveTexture(GL_TEXTURE0);	
	glBindTexture(GL_TEXTURE_2D, GetTexture(0));	

	GLuint shadowMapLocation = glGetUniformLocation(program_, "shadowMap");
	glUniform1i(shadowMapLocation, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GetTexture(1));


	//draw
	//한번만 그릴 데이터를 전부 요구한다.(vao_) 이후 glDrawElements를 통해 그린다.
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, (GLsizei)indicies_.size(), GL_UNSIGNED_INT, 0);

	//marks end of draw function
	glBindVertexArray(0);
	glUseProgram(0);
}
