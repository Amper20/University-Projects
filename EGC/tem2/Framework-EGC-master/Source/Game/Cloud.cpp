#include "Cloud.h"
#include "Transform3D.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <tuple>

using namespace std;

Cloud::Cloud(){

	gameTime = 0;

	mesh = new Mesh("Cloud");
	mesh->LoadMesh(RESOURCE_PATH::MODELS, "cloud.obj");

	shader = new Shader("ShaderLab7");
	shader->AddShader("Source/Game/Shaders/CloudVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Game/Shaders/CloudFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	
	int randomCnt = rand() % 5 + 2;
	for (int i = 0; i <= randomCnt; i++) {
		rndmPoz.push_back(glm::vec3((float)(rand() % 100) / 500.0, (float)(rand() % 100) / 500.0, (float)(rand() % 100) / 500.0));
		rndmScale.push_back( (float)(rand() % 100) / 1000.0);
		rndmColor.push_back(bodyColor - glm::vec3((float)(rand() % 100) / 1000.0, (float)(rand() % 100) / 1000.0, (float)(rand() % 100) / 1000.0));
	}
}

Cloud::~Cloud(){}

void Cloud::UpdatePoz() {}

void Cloud::Update(float deltaTimeSeconds){
	gameTime += deltaTimeSeconds;
	UpdatePoz();
}

vector<tuple<Mesh*, glm::mat4, Shader*, glm::vec3>> Cloud::getMesh() {
	
	vector< tuple<Mesh*, glm::mat4, Shader*, glm::vec3>> ret;
	
	for (int i = 0; i < rndmPoz.size(); i++) {
		modelMatrix = Transform3D::RotateOX(gameTime + randomRot);
		modelMatrix *= Transform3D::Translate(rndmPoz[i][0], rndmPoz[i][1], rndmPoz[i][2]);
		modelMatrix *= Transform3D::Translate(-2, 0, 2+randomH);
		modelMatrix *= Transform3D::Scale(0.1 + rndmScale[i], 0.1 + rndmScale[i], 0.1 + rndmScale[i]);
		ret.push_back(make_tuple(mesh, modelMatrix, shader, rndmColor[i]));
	}

	return ret;
}

