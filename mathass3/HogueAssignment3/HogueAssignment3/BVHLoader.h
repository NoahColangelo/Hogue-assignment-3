#pragma once

#include <iostream>
#include <fstream>
#include "Joint.h"
#include "GameObject.h"

using std::cout;
using std::endl;
using std::ifstream;

class GameObject;

class BVHLoader
{
public:
	BVHLoader();

	bool loadBVH(const string& fileName);
	Joint* getJointInfoByName(const string& jointName) const;
	Transform* getGameObjectByName(const string& jointName) const;
	void createGameObjects();
	Transform* getRootGameObject();
	vector<GameObject*> getGameObjects() const;
	vector<GameObject*> _jointGameObjects;

	int getNumOfFrames() const;

private:
	// Parsing helper functions
	bool parseHierarchySection(ifstream& file);
	bool parseJoint(ifstream& file, const string& parentName, Joint* currentJoint);
	bool parseMotion(ifstream& file);


	// Desrcibes the data in the file
	int _numOfFrames;
	float _fps;

	RotationOrder _rotationOrder;
	char _upAxis;
	string _rotationUnits;
	char _boneLengthAxis;

	// Animation Data
	vector<Joint*> _joints;
	GameObject* _rootGameObject;
};