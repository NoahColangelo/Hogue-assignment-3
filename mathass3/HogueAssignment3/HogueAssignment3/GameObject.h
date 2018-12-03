#pragma once

#include "Transform.h"
#include "BVHLoader.h"
#include "Camera.h"
#include "ShaderProgram.h"

class BVHLoader;

class GameObject : public Transform
{

	friend class BVHLoader;

public:
	GameObject();
	~GameObject();

	bool AnimPlaying;

	virtual void update(float dt);
	void initializeSkeletonFromBVH(std::string BVHFilePath);

protected:

	int CurrentFrame;
	BVHLoader* BVHAnimation;
	Joint* JointAnimation; // The animation for this specific joint


};