#include "GameObject.h"

GameObject::GameObject()
	: CurrentFrame(0),
	BVHAnimation(nullptr),
	JointAnimation(nullptr),
	AnimPlaying(true)
{
	_shaderProgram = nullptr;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

GameObject::~GameObject()
{
	delete BVHAnimation;
	delete JointAnimation;
}

void GameObject::update(float deltaTime)
{
	// Game object has no joints.
	if (!JointAnimation)
	{
		Transform::update(deltaTime);
	}
	else
	{
		if (CurrentFrame >= JointAnimation->numOfFrames)
			CurrentFrame = 0;

		mat4 baseRotation;
		if (JointAnimation->jointName == "End Site")
		{
			baseRotation = mat4_cast(quat());
			LocalRotation = baseRotation;
		}
		else
			mat4 baseRotation = mat4_cast(JointAnimation->jointBaseRotation);
		mat4 baseTranslation = glm::translate(JointAnimation->offset);
		mat4 bindSpaceTransform = baseTranslation * baseRotation;

		if (CurrentFrame != -1)
		{
			if (JointAnimation->jointName != "End Site")
				LocalRotation = mat4_cast(JointAnimation->jointBaseRotation * JointAnimation->jointRotations[CurrentFrame]);


			if (JointAnimation->jointParent == "")
				LocalPosition = JointAnimation->offset * JointAnimation->jointPositions[CurrentFrame];
			else
				LocalPosition = JointAnimation->offset;

			Scale = vec3(1.0f);

			// Create translation matrix
			mat4 tran = glm::translate(LocalPosition);

			// Create scale matrix
			mat4 scal = scale(Scale);

			LocalTransformMatrix = tran * LocalRotation * scal;
		}


		if (Parent)
		{
			LocalToWorldMatrix = Parent->getLocalToWorldMatrix() * LocalTransformMatrix;
		}
		else
		{
			LocalToWorldMatrix = LocalTransformMatrix;
		}
		worldPosition = vec3(LocalToWorldMatrix[3][0], LocalToWorldMatrix[3][1], LocalToWorldMatrix[3][2]);

		for (Transform* child : Children)
			child->update(deltaTime);

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

 // Add the root node from the BVH file as a child to
 // this GameObject. This allows us to control the locomotion of the hierarchy
 // my changing 'this' GameObject's scale, rotation and translation
	addChild(BVHAnimation->getRootGameObject());
	
	dynamic_cast<GameObject*>(Children[0])->getJointInfo(BVHAnimation->getNumOfFrames());
}

void GameObject::addAnimation(BVHLoader * animation)
{
	animations.push_back(animation);
}

void GameObject::draw(Camera& camera)
{
	if (Parent)
	{
		float* uview = (float*)(glm::value_ptr(camera.getinverse()));

		_shaderProgram->bind();
		_shaderProgram->sendUniformMat4("uModel", (float*)(glm::value_ptr(getLocalToWorldMatrix())), false);
		_shaderProgram->sendUniformMat4("uView", uview, false);
		_shaderProgram->sendUniformMat4("uProj", (float*)(glm::value_ptr(camera.getProjection())), false);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		float positions[6];
		positions[0] = Parent->worldPosition.x;
		positions[1] = Parent->worldPosition.y;
		positions[2] = Parent->worldPosition.z;
		positions[3] = this->worldPosition.x;
		positions[4] = this->worldPosition.y;
		positions[5] = this->worldPosition.z;

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2, positions, GL_DYNAMIC_DRAW);

		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(GL_NONE);
		glBindBuffer(GL_ARRAY_BUFFER,GL_NONE);

		_shaderProgram->unBind();
	}
}

void GameObject::getJointInfo(int frames)
{
	vector<channels> channelOrder = JointAnimation->Channels;
	vector<vector<float>> channelData = JointAnimation->motionData;
	vec3 position;
	vec3 angles;
	quat quatAngles;
	string rotOrder;
	unsigned int count = 0;

	// Loop through each frame
	for (unsigned int i = 0; i < JointAnimation->numOfFrames; i++)
	{
		// For each frame process data for each channel
		for (unsigned int j = 0; j < channelOrder.size(); j++)
		{
			// Populate position or rotation
			channels channel = channelOrder[j];

			switch (channel)
			{
			case channels::xPosition:
			{
				position.x = channelData[i][j];
				break;
			}
			case channels::yPosition:
			{
				position.y = channelData[i][j];
				break;
			}
			case channels::zPosition:
			{
				position.z = channelData[i][j];
				break;
			}
			case channels::xRotation:
			{
				angles.x = channelData[i][j];
				rotOrder.push_back('X');
				break;
			}
			case channels::yRotation:
			{
				angles.y = channelData[i][j];
				rotOrder.push_back('Y');
				break;
			}
			case channels::zRotation:
			{
				angles.z = channelData[i][j];
				rotOrder.push_back('Z');
				break;
			}
			default:
			{
				cout << "Could not process joint data!" << endl;
				system("pause");
				break;
			}
			}

			count++;

			// Check if we have a complete position or rotation
			if (count == 3)
			{
				if (rotOrder.size() == 3)
				{
					quatAngles = createQuat(angles, rotOrder);
					JointAnimation->jointRotations.push_back(quatAngles);
					rotOrder.clear();
				}
				else
				{
					JointAnimation->jointPositions.push_back(position);
				}

				count = 0;
			}
		}
	}


	//Do this for each child
	for (Transform* child : Children)
	{
		dynamic_cast<GameObject*>(child)->getJointInfo(JointAnimation->numOfFrames);
	}

}

quat GameObject::createQuat(const vec3 & rotation, const string & order)
{
	quat qX = angleAxis(glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
	quat qY = angleAxis(glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
	quat qZ = angleAxis(glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));

	if (order == "XYZ")
		return (qX * qY * qZ);
	else if (order == "YZX")
		return (qY * qZ * qX);
	else if (order == "ZXY")
		return (qZ * qX * qY);
	else if (order == "XZY")
		return (qX * qZ * qY);
	else if (order == "YXZ")
		return (qY * qX * qZ);
	else if (order == "ZYX")
		return (qZ * qY * qX);
	else
	{
		cout << "Invalid rotation order!" << endl;
		system("pause");
	}

	return qZ * qY * qX;
}
