#include "pch.h"
#include "Camera.h"

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
