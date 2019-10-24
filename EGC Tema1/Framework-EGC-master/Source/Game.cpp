#include "Game.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;


	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	for (auto it : bird.getMeshes()) {
		AddMeshToList(it.first);
	}
	for (auto it : obstacle.getMeshes()){
		cout << (it.first)->GetMeshID << "\n";
		AddMeshToList(it.first);
	}
}

void Game::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(135.0f/255, 206.0f/255,	250.0f/255, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::UpdateObjects(float deltaTimeSeconds) {
	
	bird.updateSpeed(g*deltaTimeSeconds);
	bird.updatePoz(deltaTimeSeconds);
}

void Game::Update(float deltaTimeSeconds)
{
	UpdateObjects(deltaTimeSeconds);
	Render();
}

void Game::Render() {

	modelMatrix = glm::mat3(1)*Transform2D::Translate(bird.translateX, bird.translateY);
	modelMatrix *= Transform2D::Scale(0.5, 0.5);
	for (auto it : bird.getMeshes()) {
		RenderMesh2D(it.first, shaders["VertexColor"], modelMatrix*it.second);
	}

	for (auto it : obstacle.getMeshes()) {
		RenderMesh2D(it.first, shaders["VertexColor"], it.second);
	}
}

void Game::FrameEnd()
{

}

void Game::OnInputUpdate(float deltaTime, int mods)
{

}

void Game::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) {
		bird.updateSpeed(200);
	}
}

void Game::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Game::OnWindowResize(int width, int height)
{
}
