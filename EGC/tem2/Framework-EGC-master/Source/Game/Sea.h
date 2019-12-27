#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <tuple>

using namespace std;

class Sea{

	public:
		Sea();
		~Sea();

		void Update(float deltaTimeSeconds);
		void UpdatePoz();
		tuple<Mesh*, glm::mat4, Shader*> getMesh();

		glm::mat4 modelMatrix = glm::mat4(1);
		glm::mat4 baseModelMatrix = glm::mat4(1);
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		float gameTime;
		float colorR = 0.0, colorG = 0.0, colorB = 1.0;
		Mesh* mesh;
		Shader *shader;
};
