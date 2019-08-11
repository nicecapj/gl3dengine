#include "pch.h"
#include "Camera.h"
#include <GLFW/glfw3.h>	//OpenGL, window and input

Camera::Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos)
{
    fov_ = fov;
    camPos_ = camPos;

    glm::vec3 camFront = { 0.0f, 0.0f, 0.0f };
    glm::vec3 camUp = { 0.0f, 1.0f, 0.0f };

    viewMatrix_ = glm::lookAt(camPos_, camFront, camUp);
    projectionMatrix_ = glm::perspective(fov_, width / height, nearPlane, farPlane);
}


Camera::~Camera()
{
}

void Camera::ProcessKeyboard(int key, double deltatime)
{		
	switch (key)
	{
	case GLFW_KEY_UP: {
		this->camPos_.z += moveSpeed_ * deltatime;
	}break;
	case GLFW_KEY_DOWN: {
		this->camPos_.z -= moveSpeed_ * deltatime;
	}break;
	case GLFW_KEY_LEFT: {
		this->camPos_.x -= moveSpeed_ * deltatime;
	}break;
	case GLFW_KEY_RIGHT:  {
		this->camPos_.x += moveSpeed_ * deltatime;
	}break;

	}
}

void Camera::ProcessMouseMovement(double x, double y)
{
	//need implements
}

glm::mat4 Camera::GetViewMatrix()
{
    return viewMatrix_;
}

glm::mat4 Camera::GetProjectMatrix()
{
    return projectionMatrix_;
}

glm::vec3 Camera::GetCameraPosition()
{
    return camPos_;
}
