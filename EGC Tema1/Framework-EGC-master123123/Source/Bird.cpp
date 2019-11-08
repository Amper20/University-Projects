#include "Bird.h"
#include "Game.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Component/SimpleScene.h"
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Bird::Bird(){

	glm::vec3 corner = glm::vec3(0,0,0);
	float side = 100;

	modelMatrix *= Transform2D::Translate(0 , 0);
	modelMatrix *= Transform2D::Scale(0.8f, 0.7f);
	birdBody.push_back(make_pair(Object2D::CreateTriangle("wing", corner, side, glm::vec3(1, 0.8, 0)), modelMatrix));

	modelMatrix = glm::mat3(1);
	birdBody.push_back(make_pair( Object2D::CreateSquare("body", corner, side, glm::vec3(1, 1, 0), true), modelMatrix));
	
	modelMatrix *= Transform2D::Translate(0,side);
	modelMatrix *= Transform2D::Scale(1.0f, 0.7f);
	birdBody.push_back(make_pair(Object2D::CreateTriangle("comb", corner, side, glm::vec3(1, 0, 0)),modelMatrix));

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Rotate(-float(M_PI)/2.0f);
	modelMatrix *= Transform2D::Translate(-side, side);
	modelMatrix *= Transform2D::Scale(0.5f,0.7f);
	birdBody.push_back(make_pair(Object2D::CreateTriangle("nose", corner, side, glm::vec3(1.0, 0.5, 0.0)), modelMatrix));

}

Bird::~Bird(){}

void Bird::updateSpeed(float speed){
	this->speed += speed;
}
void Bird::updateRot(float time) {
	rot += (speed > 0 ? time/3.0f : -time/3.0f);
}
void Bird::updatePoz(float time) {
	translateY += time * this->speed;
}

void Bird::Reset() {
	speed = 0;
	rot = 0.2f;
	translateY = 250.0f;
}

vector<pair<Mesh *, glm::mat3>> Bird::getMeshes() {

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(translateX, translateY);
	modelMatrix *= Transform2D::Scale(0.5, 0.5);
	modelMatrix *= Transform2D::Rotate(rot);
	birdBodyRet.clear();
	for (auto it : birdBody) {
		glm::mat3 aux = glm::mat3(1);
		if (strcmp(it.first->GetMeshID(), "wing") == 0) 
			aux = Transform2D::Scale(1.0f, (-(rot-0.3f)*2.0f));
		birdBodyRet.push_back(make_pair(it.first, modelMatrix*aux*it.second));
	}

	return birdBodyRet;
	
}
