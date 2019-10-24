#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>

using namespace std;

class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	void updateSpeed(float speed);
	void updatePoz(float time);
	void updateOrigin(float time);
	vector<pair<double, double>> origin;
	vector<pair<Mesh *, glm::mat3>> getMeshes();
	float translateX = 650, translateY = 250;
	float spawn = 0;

protected:
	glm::mat3 modelMatrix;
	float speed = 0;
	vector<pair<Mesh *, glm::mat3>> obstacleMesh;
};
