#include "Camera.h"

Camera::Camera()
{
	perspective(60.0f, 1.0f, 0.1f, 100.0f); 
}

Camera::Camera(ProjectionType projType)
{
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f); 
}

void Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	projectionType = ProjectionType::Perspective;

}

mat4 Camera::getView() const
{
	return getLocalToWorldMatrix();
}

mat4 Camera::getProjection() const
{
	return projection;
}

ProjectionType Camera::getProjType() const
{
	return projectionType;
}
