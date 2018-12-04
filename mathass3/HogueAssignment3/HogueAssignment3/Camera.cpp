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

	//fovy = fovy * 180.0f / 3.14f;
	//mat4 result;
	//result[0].x = 1.0f / (aspect * tanf(fovy / 2.0f));
	//result[0].y = 0.f;
	//result[0].z = 0.f;
	//result[0].w = 0.f;
	//result[1].x = 0.f;
	//result[1].y = 1.0f / tanf(fovy / 2.0f);
	//result[1].z = 0.f;
	//result[1].w = 0.f;
	//result[2].x = 0.f;
	//result[2].y = 0.f;
	//result[2].z = -((zFar + zNear) / (zFar - zNear));
	//result[2].w = -((2.0f * zFar * zNear) / (zFar - zNear));
	//result[3].x = 0.f;
	//result[3].y = 0.f;
	//result[3].z = -1.0f;
	//result[3].w = 0.f;

	//projection = result;
}

mat4 Camera::getView()
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

mat4 Camera::getinverse()
{
	return glm::inverse(getLocalToWorldMatrix());
}
