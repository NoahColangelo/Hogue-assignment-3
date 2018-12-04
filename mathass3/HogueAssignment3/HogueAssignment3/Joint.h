#pragma once

#include<GLM/glm/glm.hpp>
#include <GLM/glm/gtc/quaternion.hpp>
#include <GLM/glm/gtx/quaternion.hpp>
#include<vector>

enum RotationOrder
{
	XYZ = 0,
	ZYX = 0,
	XZY = 0,
	YXZ = 0,
	YZX = 0
};

enum channels
{
	xPosition,
	yPosition,
	zPosition,
	xRotation,
	yRotation, 
	zRotation
};

struct Joint
{
	Joint()
	{
		numOfFrames = 0;
	}

	std::string jointName;			// Name of joint
	std::string jointParent;		// Name of joint's parent
	glm::quat jointBaseRotation;	// Rotation of joint at frame 0
	glm::vec3	offset;	// Position of joint at frame 0
	

	std::vector<glm::vec3>	jointPositions;		// Joint point for each frame (should all be zero except for root)
	std::vector<glm::quat> jointRotations;	// Joint rotations for each frame 
	std::vector<float> jointScales;
	std::vector<channels> Channels;
	std::vector<std::vector<float>> motionData;

	unsigned int numOfFrames;
};