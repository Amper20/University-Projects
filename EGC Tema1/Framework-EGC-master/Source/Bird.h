#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>

using namespace std;

class Bird
{
public:
	Bird();
	~Bird();
	void updateSpeed(float speed);
	void updatePoz(float time);
	vector<pair<Mesh *, glm::mat3>> getMeshes();
	float translateX = 650, translateY = 250;
protected:
	glm::mat3 modelMatrix;
	float speed = 0;
	vector<pair<Mesh *, glm::mat3>> birdBody;
};
