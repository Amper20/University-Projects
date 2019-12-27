#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <tuple>

using namespace std;

class Plane{

	public:
		Plane();
		~Plane();

		void Update(float deltaTimeSeconds);
		void UpdateDead(float deltaTimeSeconds);
		void UpdatePoz();
		tuple<Mesh*, glm::mat4, Shader*> getPlane();
		tuple<Mesh*, glm::mat4, Shader*> getFuel();
		tuple<Mesh*, glm::mat4, Shader*> getRotor();
		void update(float Y);
		glm::vec3 getPoz();

		glm::mat4 modelMatrix = glm::mat4(1);
		glm::mat4 baseModelMatrix = glm::mat4(1);
		unsigned int materialShininess;
		float yPoz = 0, dY = 0, vY = 1.0;
		float materialKd;
		float materialKs;
		float gameTime;
		float fuel = 100;
		bool dead = 0;
		glm::vec3 rotorColor = glm::vec3(0, 0, 0);
		glm::vec3 bodyColor = glm::vec3(1.0, 0, 0);
		Mesh *mesh, *rotor, * fuelMesh;
		Shader *shader, *fuelShader;
};
