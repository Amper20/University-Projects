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

Obstacle::Obstacle()
{
	glm::vec3 corner = glm::vec3(0,0,0);
	float width = 100, length = 750;

	modelMatrix = glm::mat3(1);
	obstacleMesh.push_back(make_pair( Object2D::CreateRect("body", corner, length, width, glm::vec3(0, 1, 0), true), modelMatrix));

}

Obstacle::~Obstacle()
{
}

void Obstacle::updateSpeed(float speed) {
	this->speed += speed;
}

void Obstacle::updateOrigin(float time) {
	spawn -= time;
	if (spawn <= 0) {
		spawn = timeSpawn;
		double x1 = rand()%300+40;
		double x2 = rezy - opening;
		origin.push_back(make_pair(x1, rezx));
		origin.push_back(make_pair(x2, rezx));
	}
}

void Obstacle::updatePoz(float time) {
	for (auto it : origin) {
		it.first -= time * speed;
	}
	for (auto it = origin.begin(); it != origin.end();)
	{
		if (it->first < - 200)
			it = origin.erase(it);
		else
			++it;
	}
}

vector<pair<Mesh *, glm::mat3>> Obstacle::getMeshes() {
	return obstacleMesh;
}
