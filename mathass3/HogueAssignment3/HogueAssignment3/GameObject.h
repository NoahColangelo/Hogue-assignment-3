#pragma once

#include "Transform.h"
#include "BVHLoader.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include <GLM/glm/gtc/matrix_inverse.hpp>

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
	void addAnimation(BVHLoader* animation);
	void draw(Camera& camera);
	void getJointInfo(int frames);
	quat createQuat(const vec3& rotation, const string& order);

	GLuint VAO = GL_NONE;
	GLuint VBO = GL_NONE;

	ShaderProgram* _shaderProgram;
	BVHLoader* BVHAnimation;
	vector<BVHLoader*> animations;

protected:

	int CurrentFrame;

	Joint* JointAnimation; // The animation for this specific joint

};