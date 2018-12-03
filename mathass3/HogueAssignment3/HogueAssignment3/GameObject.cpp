#include "GameObject.h"

GameObject::GameObject()
	: CurrentFrame(0),
	BVHAnimation(nullptr),
	JointAnimation(nullptr),
	AnimPlaying(true)
{
}

GameObject::~GameObject()
{
	delete BVHAnimation;
	delete JointAnimation;
}

void GameObject::update(float dt)
{
	// This GameObject has no jointAnimation so we will construct our
	// transformation matrices in the traditional way
	if (JointAnimation == nullptr)
	{
		if (!AnimPlaying)
			dt = 0.0f;

		Transform::update(dt);
	}
	else
	{
		//loop in the animation if we reach the last keyframe
		if ((CurrentFrame) >= (int)JointAnimation->numOfFrames)
			(CurrentFrame) = 0;

		glm::mat4 baseRotation = glm::mat4_cast(JointAnimation->jointBaseRotation);
		glm::mat4 baseTranslation = glm::translate(JointAnimation->jointBasePosition);
		glm::mat4 bindSpaceTransform = baseTranslation * baseRotation;

		if (CurrentFrame != -1)
		{
			//// Todo: create localRotation, scale and translation matrices using HTR data

			LocalRotation =
				glm::mat4_cast(JointAnimation->jointBaseRotation *
					JointAnimation->jointRotations[CurrentFrame]);

			LocalPosition = JointAnimation->jointBasePosition +
				JointAnimation->jointPositions[CurrentFrame];

			LocalScale = glm::vec3(JointAnimation->jointScales[CurrentFrame]);

			// Create translation matrix
			glm::mat4 tran = glm::translate(LocalPosition);

			// Create scale matrix
			glm::mat4 scal = glm::scale(LocalScale);

			LocalTransformMatrix = tran * LocalRotation * scal;
		}
		else


		if (Parent)
		{
			LocalToWorldMatrix = Parent->getLocalToWorldMatrix() * LocalToWorldMatrix;
		}
		else
		{
			LocalToWorldMatrix = LocalTransformMatrix;
		}

		// Update children
		for (int i = 0; i < Children.size(); i++)
			Children[i]->update(dt);

		// Increment frame to next frame
		// Note: Right now we are just playing the animation back frame by frame
		// Since we exported the animation at 60 fps, as long as the game runs at 60 fps this works fine
		// However, to make this more robust, you should blend between frames (i.e. lerp / slerp) to get
		// frame rate independent playback. This will also allow you to control the
		// playback speed of the animation (hint: this is what you need to do for assignment 3!)
		CurrentFrame++;
	}
}

void GameObject::initializeSkeletonFromBVH(std::string BVHFilePath)
{
// Load the HTR data
	BVHAnimation = new BVHLoader();
	BVHAnimation->loadBVH(BVHFilePath); 

 // Create GameObjects from the data
	BVHAnimation->createGameObjects(); 

 // Add the root node from the HTR file as a child to
 // this GameObject. This allows us to control the locomotion of the hierarchy
 // my changing 'this' GameObject's scale, rotation and translation
	addChild(BVHAnimation->getRootGameObject());
}
