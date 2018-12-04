#pragma once
#include "Joint.h"
#include <string>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtx/transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>
#include <vector>

using glm::mat4;
using glm::vec3;
using glm::vec4;
using std::string;
using std::vector;

class Transform
{
public:
	vec3 Scale = vec3(1.0f);
	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;

	vec3 LocalPosition;

	mat4 LocalRotation;
	mat4 LocalToWorldMatrix;

	Transform();

	void setPosition(vec3 newPosition);
	void setRotationAngleX(float newAngle);
	void setRotationAngleY(float newAngle);
	void setRotationAngleZ(float newAngle);
	void setScale(float newScale);

	vec3 getPosition();
	float getRotationAngleY();

	mat4 getLocalToWorldMatrix();
	mat4 LocalTransformMatrix;

	virtual void update(float dt);
	virtual void draw();

	std::vector <Transform*> Children;
	Transform* Parent;

protected:
	// Other Properties
	std::string name;
	vec4 color;
};