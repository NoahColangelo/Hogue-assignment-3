#include "BVHLoader.h"


BVHLoader::BVHLoader()
{
}

bool BVHLoader::loadBVH(const string & fileName)
{
	ifstream file;
	file.open(fileName);

	if (!file.is_open())
	{
		cout << "Could not open BVH file!" << endl;
		system("pause");
		return false;
	}

	string token;
	file >> token;

	if (token == "HIERARCHY")
	{
		if (!parseHierarchySection(file))
		{
			cout << "Could not find hierarchy at the top of the file!" << endl;
			system("pause");
			return false;
		}
	}

	return true;
}

Joint * BVHLoader::getJointInfoByName(const string & jointName) const
{
	for (Joint* jointInfo : _joints)
	{
		if (jointInfo->jointName == jointName)
			return jointInfo;
	}

	return nullptr;
}

Transform * BVHLoader::getGameObjectByName(const string & jointName) const
{
	for (GameObject* gameObject : _jointGameObjects)
	{
		if (gameObject->name == jointName)
			return gameObject;
	}

	return nullptr;
}

void BVHLoader::createGameObjects()
{
	// Create a game object for each joint
	for (Joint* jointInfo : _joints)
	{
		GameObject* newJoint = new GameObject();
		newJoint->name = jointInfo->jointName;
		newJoint->JointAnimation = jointInfo;

		_jointGameObjects.push_back(newJoint);
	}

	// Set up the hierarchy
	for (Joint* jointInfo : _joints)
	{
		jointInfo->numOfFrames = _numOfFrames;

		Transform* child = getGameObjectByName(jointInfo->jointName);
		Transform* parent = getGameObjectByName(jointInfo->jointParent);

		if (!child || !parent)
			continue;

		parent->addChild(child);
	}
}

Transform * BVHLoader::getRootGameObject()
{
	if (!_rootGameObject)
	{
		_rootGameObject = _jointGameObjects.front();
	}

	return _rootGameObject;
}

vector<GameObject*> BVHLoader::getGameObjects() const
{
	return _jointGameObjects;
}

int BVHLoader::getNumOfFrames() const
{
	return _numOfFrames;
}

bool BVHLoader::parseHierarchySection(ifstream & file)
{
	string token;

	if (file.good())
	{
		// Go to next word after "HIERARCHY"
		file >> token;

		if (token == "ROOT")
		{
			Joint* rootJoint = new Joint();

			// Parse root joint, and all of it's children
			if (!parseJoint(file, "", rootJoint))
			{
				cout << "Could not parse joint!" << endl;
				system("pause");
				return false;
			}

			//_jointInfos.push_back(rootJoint);
		}
		else
		{
			cout << "No root joint found, but found: " << token << endl;
			system("pause");
			return false;
		}
	}

	if (file.good())
	{
		file >> token; // Motion

		if (token == "MOTION")
		{
			if (!parseMotion(file))
			{
				cout << "Could not parse motion!" << endl;
				system("pause");
				return false;
			}
		}
		else
		{
			cout << "No motion found, but found: " << token << endl;
			system("pause");
			return false;
		}
	}

	// Hierachy was successfully parsed
	return true;
}

bool BVHLoader::parseJoint(ifstream & file, const string & parentName, Joint * currentJoint)
{
	// Set joint's parent's name
	currentJoint->jointParent = parentName;

	string name;
	file >> name;
	currentJoint->jointName = name;

	string token;

	file >> token; // Skip "{"
	file >> token; // Offset

	if (token == "OFFSET")
	{
		// Store offset
		vec3 offset;
		//sscanf(token.c_str(), "%*s %f %f %f", &offset.x, &offset.y, &offset.z);
		file >> offset.x >> offset.y >> offset.z;
		currentJoint->offset = offset;
	}
	else
	{
		cout << "Invalid Offset!" << endl;
		system("pause");
		return false;
	}


	file >> token; // Channels

	 
	if (token != "CHANNELS")
	{
		cout << "Invalid Number Of Channels!" << endl;
		system("pause");
		return false;
	}

	// Store channels
	unsigned int numOfChannels;
	//sscanf(buffer, "%*s %u", &numOfChannels);
	file >> numOfChannels;
	
	for (unsigned int i = 0; i < numOfChannels; i++)
	{
		file >> token;

		if (token == "Xposition")
		{
			currentJoint->Channels.push_back(channels::xPosition);
		}
		else if (token == "Yposition")
		{
			currentJoint->Channels.push_back(channels::yPosition);
		}												
		else if (token == "Zposition")					
		{											
			currentJoint->Channels.push_back(channels::zPosition);
		}
		else if (token == "Xrotation")
		{
			currentJoint->Channels.push_back(channels::xRotation);
		}												
		else if (token == "Yrotation")					
		{												
			currentJoint->Channels.push_back(channels::yRotation);
		}												
		else if (token == "Zrotation")					
		{												
			currentJoint->Channels.push_back(channels::zRotation);
		}
		else
		{
			cout << "Invalid Channel!" << endl;
			system("pause");
			return false;
		}
	}


	file >> token;


	_joints.push_back(currentJoint);

	
	while (file.good())
	{
		// Found a joint
		if (token == "JOINT")
		{
			Joint* child = new Joint();
			if (!parseJoint(file, currentJoint->jointName, child))
			{
				cout << "Could not parse child joint!" << endl;
				system("pause");
				return false;
			}

			//_jointInfos.push_back(child);
		}
		// Found an end joint
		else if (token == "End")
		{
			file >> token >> token; // Skip "Site {"

			Joint* endJoint = new Joint();

			endJoint->jointParent = (currentJoint->jointName);
			endJoint->jointName = "End Site";

			file >> token;

			if (token == "OFFSET")
			{
				vec3 offset;
				file >> offset.x >> offset.y >> offset.z;
			}
			else
			{
				cout << "Invalid End Offset!" << endl;
				system("pause");
				return false;
			}

			_joints.push_back(endJoint);

			file >> token; // Skip "}"
		}
		// End of a joint section
		else if (token == "}")
		{
			return true;
		}

		file >> token;
	}

	return false;
}

bool BVHLoader::parseMotion(ifstream & file)
{
	string token;
	file >> token; // Number of frames

	int numOfFrames;

	if (token == "Frames:")
	{
		file >> numOfFrames;
		_numOfFrames = numOfFrames;
	}
	else
	{
		cout << "Invalid number of frames! Found: " << token << endl;
		system("pause");
		return false;
	}

	file >> token; // Frame time

	float frameTime;

	if (token == "Frame")
	{
		file >> token; // Skip "Time:"

		file >> frameTime;
		_fps = 1.0f / frameTime;


		float value;
		// Loop through each frame
		for (int i = 0; i < numOfFrames; i++)
		{
			// Loop through each joint
			//for (JointInfo* jointInfo : _jointInfos)
			//{
			//	vector<float> data;
			//	// Loop through each channel
			//	for (unsigned int j = 0; j < jointInfo->getNumOfChannels(); j++)
			//	{
			//		file >> value;
			//		data.push_back(value);
			//	}
			//	jointInfo->addFrameMotionData(data);
			//}

			vector<Joint*>::iterator it;
			for (it = _joints.begin(); it != _joints.end(); it++)
			{
				vector<float> data;
				// Loop through each channel
				for (unsigned int j = 0; j < (*it)->Channels.size(); j++)
				{
					file >> value;
					data.push_back(value);
				}
				(*it)->motionData.push_back(data);
			}
		}
	}
	else
	{
		cout << "Invalid frame time! Found: " << token << endl;
		system("pause");
		return false;
	}


	return true;
}
