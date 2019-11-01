#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>

using namespace std;

class Obstacle{

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
	vector<pair<double, double>> getOrigin();
	bool checkColision(glm::mat3 mm, float x, float y);
	void Reset();

protected:
	glm::mat3 modelMatrix;
	double speed = 200.0f;
	float widthObstacle = 100, lengthObstacle = 750;
	vector<pair<Mesh *, glm::mat3>> obstacleMesh;
};
