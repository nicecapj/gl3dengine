#pragma once
#include <GL/glew.h>
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos);
    ~Camera();

	void ProcessKeyboard(int key, double deltatime);
	void ProcessMouseMovement(double x, double y);	
	void SetMoveSpeed(float moveSpeed);

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectMatrix();	

    glm::vec3 GetCameraPosition();
	void SetPosition(glm::vec3 pos);	
	void SetTargetPosition(glm::vec3 pos);

	void SetOrthProjection(float left, float right, float top, float bottom, float nearPlan, float farPlan);
	void SetPerspectiveProjection(GLfloat fov, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane);
private:
    glm::mat4 viewMatrix_;
    glm::mat4 projectionMatrix_;
	glm::vec3 camPos_ = { 0.0f,0.0f,0.0f };
	glm::vec3 targetPos_ = { 0.0f,0.0f,0.0f };
    GLfloat fov_;

	bool isPerspetive = true;

	float moveSpeed_ = 1000.0f;
};

