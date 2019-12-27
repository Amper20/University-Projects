#include "Live.h"
#include "Transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <tuple>

using namespace std;

Live::Live(){

	gameTime = 0;

	mesh = new Mesh("Live");
	mesh->LoadMesh(RESOURCE_PATH::MODELS, "teapot.obj");

	shader = new Shader("Live");
	shader->AddShader("Source/Game/Shaders/LiveVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Game/Shaders/LiveFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
}

Live::~Live(){}

void Live::UpdatePoz() {}

glm::vec3 Live::getPoz() {

	modelMatrix = Transform3D::Translate(0, 0, 0);
	return modelMatrix* glm::vec4(0, 0, 0, 1);
}

void Live::Update(float deltaTimeSeconds){
	gameTime += deltaTimeSeconds;
	if (flaged)
		flaged++;
	UpdatePoz();
}

tuple<Mesh*, glm::mat4, Shader*, glm::vec3> Live::getMesh(float offset) {
	
	tuple<Mesh*, glm::mat4, Shader*, glm::vec3> ret;
	
	modelMatrix = Transform3D::Translate(-0.85 + offset, 0.85, 0);
	modelMatrix *= Transform3D::Scale(0.15, 0.25, 0.25);
	modelMatrix *= Transform3D::RotateOY(gameTime); 
	modelMatrix *= Transform3D::RotateOX(gameTime);

	if(flaged)
		ret = (make_tuple(mesh, modelMatrix, shader, bodyColorUsed));
	else
		ret = (make_tuple(mesh, modelMatrix, shader, bodyColor));

	return ret;
}

