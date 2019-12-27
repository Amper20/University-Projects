#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <tuple>

using namespace std;

class Live{

	public:
		Live();
		~Live();

		void Update(float deltaTimeSeconds);
		void UpdatePoz();
		glm::vec3 getPoz();
		tuple<Mesh*, glm::mat4, Shader*, glm::vec3> getMesh(float offset);

		glm::mat4 modelMatrix = glm::mat4(1);
		glm::mat4 baseModelMatrix = glm::mat4(1);
		
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		float gameTime;
		glm::vec3 bodyColor = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3 bodyColorUsed = glm::vec3(1.0, 0.0, 0.0);
		int flaged = 0;

		Mesh* mesh;
		Shader *shader;
		
};
