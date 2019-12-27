#include "Fuel.h"
#include "Transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <tuple>

using namespace std;

Fuel::Fuel(){

	gameTime = 0;

	mesh = new Mesh("Fuel");
	mesh->LoadMesh(RESOURCE_PATH::MODELS, "fuel.obj");

	shader = new Shader("Fuel");
	shader->AddShader("Source/Game/Shaders/FuelVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Game/Shaders/FuelFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
}

Fuel::~Fuel(){}

void Fuel::UpdatePoz() {}

glm::vec3 Fuel::getPoz() {
	modelMatrix = Transform3D::RotateOX(gameTime + randomRot);
	modelMatrix *= Transform3D::Translate(0, 2, randomH);
	return modelMatrix* glm::vec4(0, 0, 0, 1);
}

void Fuel::Update(float deltaTimeSeconds){
	gameTime += deltaTimeSeconds;
	if (flaged)
		flaged++;
	UpdatePoz();
}

tuple<Mesh*, glm::mat4, Shader*, glm::vec3> Fuel::getMesh() {
	
	tuple<Mesh*, glm::mat4, Shader*, glm::vec3> ret;
	
	modelMatrix = Transform3D::RotateOX(gameTime + randomRot);
	modelMatrix *= Transform3D::Translate(0, 2, randomH);
	modelMatrix *= Transform3D::Scale(0.05, 0.05, 0.05);
	modelMatrix *= Transform3D::RotateOY(gameTime); 
	modelMatrix *= Transform3D::RotateOX(gameTime);

	if(flaged)
		ret = (make_tuple(mesh, modelMatrix, shader, bodyColorUsed));
	else
		ret = (make_tuple(mesh, modelMatrix, shader, bodyColor));

	return ret;
}

