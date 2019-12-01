#include "Obstacle.h"
#include "Transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <tuple>

using namespace std;

Obstacle::Obstacle(){

	gameTime = 0;

	mesh = new Mesh("Obstacle");
	mesh->LoadMesh(RESOURCE_PATH::MODELS, "sphere.obj");

	shader = new Shader("Obstacle");
	shader->AddShader("Source/Game/Shaders/FuelVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Game/Shaders/FuelFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
}

Obstacle::~Obstacle(){}

void Obstacle::UpdatePoz() {}

glm::vec3 Obstacle::getPoz() {
	modelMatrix = Transform3D::RotateOX(gameTime + randomRot);
	modelMatrix *= Transform3D::Translate(0, 2, randomH);
	return modelMatrix* glm::vec4(0, 0, 0, 1);
}

void Obstacle::Update(float deltaTimeSeconds){
	gameTime += deltaTimeSeconds;
	if (flaged)
		flaged++;
	UpdatePoz();
}

tuple<Mesh*, glm::mat4, Shader*, glm::vec3> Obstacle::getMesh() {
	
	tuple<Mesh*, glm::mat4, Shader*, glm::vec3> ret;
	
	modelMatrix = Transform3D::RotateOX(gameTime + randomRot);
	modelMatrix *= Transform3D::Translate(0, 2, randomH);
	modelMatrix *= Transform3D::Scale(0.15, 0.15, 0.15);
	modelMatrix *= Transform3D::RotateOY(gameTime); 
	modelMatrix *= Transform3D::RotateOX(gameTime);
	if(!flaged)
		return (make_tuple(mesh, modelMatrix, shader, bodyColor));
	return (make_tuple(mesh, modelMatrix, shader, bodyColorUsed));

}

