#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <tuple>

#include "Sea.h"
#include "Plane.h"
#include "Cloud.h"
#include "Fuel.h"
#include "Obstacle.h"
#include "LabCamera.h"
#include "Live.h"

using namespace std;

class Game : public SimpleScene
{
	public:
		Game();
		~Game();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSea(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));
		void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));
		void RenderGame();

		void checkColisions();
		void checkFuel();

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		
		float orthoRight = 8.0f, orthoUp = 4.5f, orthoDown = -4.5f, orthoLeft = -8.0f;
		bool isOrtho = false;
		bool view = true;

		bool renderCameraTarget;

		Sea *sea;

		Plane* plane;

		int cloudCnt = 25;
		vector<Cloud*> cloud;

		int fuelCnt = 5;
		vector<Fuel*> fuel;

		int live = 3;
		vector<Live*> lives;

		int obstacleCnt = 10;
		vector<Obstacle*> obstacle;

		Laborator::Camera *camera;
		Laborator::Camera *camera1;
		float gameTime = 0, speed = 1.0f;

		glm::vec3 lightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		glm::mat4 projectionMatrix;
};
