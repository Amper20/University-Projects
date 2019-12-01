#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <tuple>

using namespace std;

class Cloud{

	public:
		Cloud();
		~Cloud();

		void Update(float deltaTimeSeconds);
		void UpdatePoz();
		vector<tuple<Mesh*, glm::mat4, Shader*, glm::vec3>> getMesh();

		glm::mat4 modelMatrix = glm::mat4(1);
		glm::mat4 baseModelMatrix = glm::mat4(1);
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		float gameTime;
		vector<glm::vec3> rndmPoz;
		vector<glm::vec3> rndmColor;
		vector<float> rndmScale;
		glm::vec3 bodyColor = glm::vec3(1.0, 1.0, 1.0);
		Mesh* mesh;
		Shader *shader;
		float randomH = (float)(rand() % 100) / 50;
		float randomRot = (float)(rand() % (314*2)) / 100;
};
