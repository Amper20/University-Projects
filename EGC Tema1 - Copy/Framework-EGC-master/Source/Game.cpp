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

void Game::Init(){

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	
	for (auto it : bird.getMeshes()) {
		AddMeshToList(it.first);
	}
	for (auto it : obstacle.getMeshes()){
		AddMeshToList(it.first);
	}
}

void Game::FrameStart(){
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
	bird.updateRot(deltaTimeSeconds);
	obstacle.updateOrigin(deltaTimeSeconds);
	obstacle.updatePoz(deltaTimeSeconds);
	updateScore(deltaTimeSeconds);

}

void Game::CheckCollision() {

	gameOver = false;

	for (auto &it : bird.getMeshes()) 
		for (auto pos : it.first->vertices) 
			gameOver |= obstacle.checkColision(it.second, pos.position.x, pos.position.y);
	
	if (gameOver)
		cout << "Game over your score is :" << score << "\nPress R\n";

}

void Game::updateScore(float bonus) {
	
	if (!gameOver) {
		score += bonus;
		if ((int(score)) % 5 == 0 && displayed != (int(score))) {
			displayed = (int(score));
			cout << "Good job your current score is :" << score << "\n";
		}
	}

}

void Game::Render() {

	for (auto it : bird.getMeshes()) {
		RenderMesh2D(it.first, shaders["VertexColor"], it.second);
	}

	for (auto it : obstacle.getMeshes()) {
		RenderMesh2D(it.first, shaders["VertexColor"], it.second);
	}
}

void Game::Update(float deltaTimeSeconds){

	if (!gameOver) {
		UpdateObjects(deltaTimeSeconds);
		CheckCollision();
	}

	Render();

}

void Game::FrameEnd(){
}

void Game::OnInputUpdate(float deltaTime, int mods){
}

void Game::OnKeyPress(int key, int mods){

	if (key == GLFW_KEY_SPACE && !gameOver)
		bird.updateSpeed(200);

	if (key == GLFW_KEY_R) {
		obstacle.Reset();
		bird.Reset();
		gameOver = false;
		score = 0;
	}
}

void Game::OnKeyRelease(int key, int mods){
	// add key release event
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){
	// add mouse move event
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods){
	// add mouse button press event
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods){
	// add mouse button release event
}

void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY){
}

void Game::OnWindowResize(int width, int height){
}