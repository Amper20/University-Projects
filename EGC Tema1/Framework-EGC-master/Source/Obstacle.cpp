#include "Obstacle.h"
#include "Game.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Component/SimpleScene.h"
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

constexpr auto rezx = 1280;
constexpr auto rezy = 720;
constexpr auto opening = 240;
constexpr auto timeSpawn = 2.5f;

Obstacle::Obstacle(){}

Obstacle::~Obstacle(){}

void Obstacle::updateSpeed(float speed) {
	this->speed += speed;
}

void Obstacle::updateOrigin(float time) {
	spawn -= time;
	if (spawn <= 0) {
		spawn = timeSpawn;
		double y1 = rand()%400+40;
		double y2 = y1 + opening;
		origin.push_back(make_pair(rezx, y1 - lengthObstacle));
		origin.push_back(make_pair(rezx, y2));
	}
}

void Obstacle::Reset() {
	origin.clear();
}

bool Obstacle::checkColision(glm::mat3 mm, float x, float y) {
	
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);
	glm::vec4 vec = glm::vec4(x, y, 0.f, 1.0f);
	glm::vec4 trans = model * vec;
	x = trans[0];
	y = trans[1];

	if (y <= 0 || y >= rezy) 
		return true;
	
	for (auto &it : origin) 
		if (x >= it.first && x <= it.first + widthObstacle) 
			if (y >= it.second && y <= it.second + lengthObstacle) 
				return true;

	return false;
}

void Obstacle::updatePoz(float time) {
	
	for (auto &it : origin) 
		it.first -= time * speed;

	for (auto &it = origin.begin(); it != origin.end();)
		if (it->first < - 200)
			it = origin.erase(it);
		else
			++it;
}

vector<pair<double, double>> Obstacle::getOrigin() {
	return origin;
}

vector<pair<Mesh *, glm::mat3>> Obstacle::getMeshes() {
	
	glm::vec3 corner = glm::vec3(0, 0, 0);
	vector<pair<Mesh *, glm::mat3>> ret;
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, rezy + widthObstacle);
	modelMatrix *= Transform2D::Rotate(-M_PI / 2);
	ret.push_back(make_pair(Object2D::CreateRect("upperLimit", corner, rezx, widthObstacle, glm::vec3(0, 1, 0), true), modelMatrix));
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Rotate(-M_PI / 2);
	ret.push_back(make_pair(Object2D::CreateRect("lowerLimit", corner, rezx, widthObstacle, glm::vec3(0, 1, 0), true), modelMatrix));
	
	for (auto it : origin) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(it.first, it.second);
		ret.push_back(make_pair(Object2D::CreateRect("middle", corner, lengthObstacle, widthObstacle, glm::vec3(0, 1, 0), true), modelMatrix));
	}

	return ret;
}
