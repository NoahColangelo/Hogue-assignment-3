#include "Transform.h"
#include "IO.h"

Transform::Transform()
	: color(vec4(1.0f))
{
}

void Transform::setPosition(vec3 newPosition)
{
	LocalPosition = newPosition;
}

void Transform::setRotationAngleX(float newAngle)
{
	RotX = newAngle;
}

void Transform::setRotationAngleY(float newAngle)
{
	RotY = newAngle;
}

void Transform::setRotationAngleZ(float newAngle)
{
	RotZ = newAngle;
}

void Transform::setScale(float newScale)
{
	Scale = vec3(newScale);
}

vec3 Transform::getPosition()
{
	return LocalPosition;
}

float Transform::getRotationAngleY()
{
	return RotY;
}

mat4 Transform::getLocalToWorldMatrix()
{
	return LocalToWorldMatrix;
}

void Transform::addChild(Transform * child)
{
	Children.push_back(child);
	child->Parent = this;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix

	mat4 rx;
	mat4 ry;
	mat4 rz;
	
	rx = glm::rotate(RotX, vec3(1, 0, 0));
	ry = glm::rotate(RotY, vec3(0, 1, 0));
	rz = glm::rotate(RotZ, vec3(0, 0, 1));

	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	LocalRotation = rz * ry * rx;

	// Create translation matrix
	mat4 tran = glm::translate(LocalPosition);

	// Create scale matrix
	mat4 scale = glm::scale(vec3(Scale));

	// Combine all above transforms into a single matrix
	LocalToWorldMatrix = tran * LocalRotation * scale;

	if (Parent)
		LocalToWorldMatrix = Parent->getLocalToWorldMatrix() * LocalTransformMatrix;
	else
		LocalToWorldMatrix = LocalTransformMatrix;

	worldPosition = vec3(LocalToWorldMatrix[3][0], LocalToWorldMatrix[3][1], LocalToWorldMatrix[3][2]);

	for (Transform* child : Children)
		child->update(dt);
}

void Transform::draw()
{
	//SAT_DEBUG_LOG("DRAWING TRANSFORM INSTEAD OF GAMEOBJECT");
}
