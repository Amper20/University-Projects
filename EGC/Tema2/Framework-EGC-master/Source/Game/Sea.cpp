#include "Sea.h"
#include "Transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <tuple>

using namespace std;

Sea::Sea(){

	gameTime = 0;

	mesh = new Mesh("Sea");
	mesh->LoadMesh(RESOURCE_PATH::MODELS, "sea2.obj");

	shader = new Shader("ShaderLab7");
	shader->AddShader("Source/Game/Shaders/SeaVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Game/Shaders/SeaFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	
	baseModelMatrix *= Transform3D::RotateOZ(M_PI/2);
}

Sea::~Sea()
{
}

void Sea::UpdatePoz() {
	modelMatrix = baseModelMatrix;
	modelMatrix *= Transform3D::RotateOY(-gameTime);
	modelMatrix *= Transform3D::Scale(1.2, 1.2, 1.2);
}

void Sea::Update(float deltaTimeSeconds){
	gameTime += deltaTimeSeconds;
	UpdatePoz();
}

tuple<Mesh*, glm::mat4, Shader*> Sea::getMesh() {
	return make_tuple(mesh, modelMatrix, shader);
}

