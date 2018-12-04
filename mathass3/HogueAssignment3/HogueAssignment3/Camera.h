#pragma once
#include "Transform.h"
#include <GLM/glm/glm.hpp>

using glm::mat4;

enum ProjectionType
{
	Perspective,
};

class Camera : public Transform
{
public:	
	Camera();
	Camera(ProjectionType projType);

	void perspective(
		float fovy, float aspect, 
		float zNear, float zFar);

	mat4 getView();
	mat4 getProjection() const;
	ProjectionType getProjType() const;
	mat4 getinverse();

private:
	ProjectionType projectionType = ProjectionType::Perspective;
	mat4 projection;
};