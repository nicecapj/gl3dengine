﻿#include "pch.h"
#include "Camera.h"
#include <GLFW/glfw3.h>	//OpenGL, window and input

Camera::Camera(GLfloat fov, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos)
{
    fov_ = fov;
    camPos_ = camPos;

    targetPos_ = { 0.0f, 0.0f, 0.0f };
    glm::vec3 camUp = { 0.0f, 1.0f, 0.0f };

    viewMatrix_ = glm::lookAt(camPos_, targetPos_, camUp);
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
		SetPosition(glm::vec3(camPos_.x, camPos_.y, camPos_.z + moveSpeed_ * deltatime));
		this->camPos_.z += moveSpeed_ * deltatime;
	}break;
	case GLFW_KEY_DOWN: {
		SetPosition(glm::vec3(camPos_.x, camPos_.y, camPos_.z - moveSpeed_ * deltatime));		
	}break;
	case GLFW_KEY_LEFT: {		
		SetPosition(glm::vec3(camPos_.x - moveSpeed_ * deltatime, camPos_.y, camPos_.z ));
	}break;
	case GLFW_KEY_RIGHT:  {
		SetPosition(glm::vec3(camPos_.x + moveSpeed_ * deltatime, camPos_.y, camPos_.z));
	}break;

	}
}

void Camera::ProcessMouseMovement(double x, double y)
{
	//need implements
}

void Camera::SetMoveSpeed(float moveSpeed)
{
	moveSpeed_ = moveSpeed;
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

void Camera::SetPosition(glm::vec3 pos)
{
	camPos_ = pos;
	viewMatrix_ = glm::lookAt(camPos_, targetPos_, glm::vec3(0, 1, 0));
}

void Camera::SetTargetPosition(glm::vec3 pos)
{
	targetPos_ = pos;
	viewMatrix_ = glm::lookAt(camPos_, targetPos_, glm::vec3(0,1,0));
}
