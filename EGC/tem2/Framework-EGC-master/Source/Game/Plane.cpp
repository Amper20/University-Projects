#include "Plane.h"
#include "Transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <tuple>

using namespace std;

Plane::Plane(){

	gameTime = 0;

	mesh = new Mesh("Plane");
	mesh->LoadMesh(RESOURCE_PATH::MODELS, "plane.obj");

	fuelMesh = new Mesh("Fuel");
	fuelMesh->LoadMesh(RESOURCE_PATH::MODELS, "box.obj");
	fuelShader = new Shader("FuelShader");
	fuelShader->AddShader("Source/Game/Shaders/LiveVertexShader.glsl", GL_VERTEX_SHADER);
	fuelShader->AddShader("Source/Game/Shaders/LiveFragmentShader.glsl", GL_FRAGMENT_SHADER);
	fuelShader->CreateAndLink();

	rotor = new Mesh("Rotor");
	rotor->LoadMesh(RESOURCE_PATH::MODELS, "box.obj");


	shader = new Shader("planeShader");
	shader->AddShader("Source/Game/Shaders/PlaneVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Game/Shaders/PlaneFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
}

Plane::~Plane()
{
}

void Plane::UpdatePoz() {}

void Plane::Update(float deltaTimeSeconds) {
	gameTime += deltaTimeSeconds;
	dY += vY * deltaTimeSeconds;
	UpdatePoz();
}
void Plane::UpdateDead(float deltaTimeSeconds) {
	gameTime += deltaTimeSeconds;
	yPoz -= 2*deltaTimeSeconds;
	UpdatePoz();
}

void Plane::update(float Y) {
	yPoz += Y;
}

glm::vec3 Plane::getPoz() {
	modelMatrix = Transform3D::Translate(0, 2.43 + yPoz, 0.34);
	modelMatrix *= Transform3D::RotateOZ(gameTime * 8);
	modelMatrix *= Transform3D::Scale(0.1, 0.02, 0.01);
	return modelMatrix * glm::vec4(0, 0, 0, 1);
}

tuple<Mesh*, glm::mat4, Shader*> Plane::getFuel() {
	
	modelMatrix = Transform3D::Translate(0.80, 0.80, 0);
	modelMatrix *= Transform3D::Scale(0.15*fuel / 100.0, 0.25*fuel/100.0, 0.25*fuel / 100.0);
	modelMatrix *= Transform3D::RotateOY(gameTime);
	modelMatrix *= Transform3D::RotateOX(gameTime);

	return make_tuple(fuelMesh, modelMatrix, fuelShader);
}

tuple<Mesh*, glm::mat4, Shader*> Plane::getPlane() {
	modelMatrix = Transform3D::Translate(0, 2.5 + yPoz, -1.5);
	if (dead) {
		modelMatrix *= Transform3D::RotateOX(yPoz);
		modelMatrix *= Transform3D::RotateOY(yPoz);
	}
	modelMatrix *= Transform3D::Scale(0.003, 0.003, 0.003);
	return make_tuple(mesh, modelMatrix, shader);
}

tuple<Mesh*, glm::mat4, Shader*> Plane::getRotor() {
	modelMatrix = Transform3D::Translate(0, 2.43 + yPoz, 0.34);
	modelMatrix *= Transform3D::RotateOZ(gameTime*8);
	if (dead) {
		modelMatrix *= Transform3D::RotateOX(yPoz);
		modelMatrix *= Transform3D::RotateOY(yPoz);
	}
	modelMatrix *= Transform3D::Scale(0.1, 0.02, 0.01);
	
	return make_tuple(rotor, modelMatrix, shader);
}

