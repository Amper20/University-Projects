#include "Game.h"

#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include <Core/Engine.h>

#include "Sea.h"
#include "Plane.h"
#include "Cloud.h"
#include "Fuel.h"
#include "Obstacle.h"
#include "LabCamera.h"

using namespace std;

Game::Game(){
	
	//camera = new Laborator::Camera();

	sea = new Sea();
	
	plane = new Plane();

	for (int i = 0; i < cloudCnt; i++) 
		cloud.push_back(new Cloud());
	
	for (int i = 0; i < fuelCnt; i++)
		fuel.push_back(new Fuel());

	for (int i = 0; i < fuelCnt; i++)
		obstacle.push_back(new Obstacle());
	GetSceneCamera()->transform->SetWorldPosition(glm::vec3(2.14781, 2.56696, 0.100131));
	//GetSceneCamera()->transform->SetWorldRotation(glm::quat(-0.079882, 0.709204, 0.0814305, 0.695717));
}

Game::~Game()
{
}

void Game::Init(){
	

	
	

	{
		lightPosition = glm::vec3(0, 2, 1);
		materialShininess = 50;
		materialKd = 1.0;
		materialKs = 1.0;
	}
}

void Game::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.52, 0.80, 0.92, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Game::RenderGame() {


	tuple<Mesh*, glm::mat4, Shader*> seaData = sea->getMesh();
	RenderSea(get<0>(seaData), get<2>(seaData), get<1>(seaData));

	tuple<Mesh*, glm::mat4, Shader*> planeData = plane->getPlane();
	RenderMesh(get<0>(planeData), get<2>(planeData), get<1>(planeData), plane->bodyColor);
	planeData = plane->getRotor();
	RenderMesh(get<0>(planeData), get<2>(planeData), get<1>(planeData), plane->rotorColor);

	for (int i = 0; i < cloud.size(); i++) {
		vector<tuple<Mesh*, glm::mat4, Shader*, glm::vec3>> cloudData = cloud[i]->getMesh();
		for (int j = 0; j < cloudData.size(); j++) {
			RenderMesh(get<0>(cloudData[j]), get<2>(cloudData[j]), get<1>(cloudData[j]), get<3>(cloudData[j]));
		}
	}

	for (int i = 0; i < fuel.size(); i++) {
		tuple<Mesh*, glm::mat4, Shader*, glm::vec3> fuelData = fuel[i]->getMesh();
		RenderMesh(get<0>(fuelData), get<2>(fuelData), get<1>(fuelData), get<3>(fuelData));
	}

	for (int i = 0; i < obstacle.size(); i++) {
		tuple<Mesh*, glm::mat4, Shader*, glm::vec3> obstacleData = obstacle[i]->getMesh();
		RenderMesh(get<0>(obstacleData), get<2>(obstacleData), get<1>(obstacleData), get<3>(obstacleData));
	}
}


void Game::checkColisions() {
	glm::vec3 planePoz = plane->getPoz();
	for (int i = 0; i < obstacle.size(); i++) {
		glm::vec3 obstaclePoz = obstacle[i]->getPoz();
		if (!obstacle[i]->flaged && glm::distance(planePoz, obstaclePoz) < 0.17) {
			live--;
			obstacle[i]->flaged = 1;
		}
	}
}

void Game::checkFuel() {
	glm::vec3 planePoz = plane->getPoz();
	for (int i = 0; i < fuel.size(); i++) {
		glm::vec3 fuelPoz = fuel[i]->getPoz();
		if (!fuel[i]->flaged && glm::distance(planePoz, fuelPoz) < 0.17) {
			plane->fuel += 30.0;
			fuel[i]->flaged = 1;
		}
	}
}

void Game::Update(float deltaTimeSeconds)
{
	if (live > 0 && plane->fuel > 0) {
		if (plane->fuel > 100.0) 
			plane->fuel = 100;
		plane->fuel -= deltaTimeSeconds * 10;
		sea->Update(deltaTimeSeconds);

		plane->Update(deltaTimeSeconds);

		for (int i = 0; i < cloud.size(); i++)
			cloud[i]->Update(deltaTimeSeconds);
		
		int remove_poz = -1;
		for (int i = 0; i < fuel.size(); i++) {
			fuel[i]->Update(deltaTimeSeconds);
			if (fuel[i]->flaged > 50)
				remove_poz = i;
		}
		if (remove_poz != -1) {
			fuel.erase(fuel.begin() + remove_poz);
			fuel.push_back(new Fuel());
		}

		remove_poz = -1;
		for (int i = 0; i < obstacle.size(); i++) {
			obstacle[i]->Update(deltaTimeSeconds);
			if (obstacle[i]->flaged > 50)
				remove_poz = i;
		}
		if (remove_poz != -1) {
			obstacle.erase(obstacle.begin() + remove_poz);
			obstacle.push_back(new Obstacle());
		}
	}
	else {
		plane->UpdateDead(deltaTimeSeconds);
	}
	checkColisions(); 
	checkFuel();
	RenderGame();
}


void Game::FrameEnd(){}



void Game::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
	}
}

void Game::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, 1.0, 1.0, 1.0);

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int gameTime = glGetUniformLocation(shader->program, "time");
	glUniform1f(gameTime, plane->gameTime);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int rnd = glGetUniformLocation(shader->program, "randm");
	glUniform3f(rnd, (float(rand() % 100)) / 10.0, (float(rand() % 100)) / 10.0, (float(rand() % 100)) / 10.0);

	int spot_poz = glGetUniformLocation(shader->program, "spot");
	glUniform1i(spot_poz, 1);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color[0], color[1], color[2]);

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Game::RenderSea(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, 1.0, 1.0, 1.0);

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int gameTime = glGetUniformLocation(shader->program, "time");
	glUniform1f(gameTime, sea->gameTime);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);
	
	int rnd = glGetUniformLocation(shader->program, "randm");
	glUniform3f(rnd, (float(rand() % 100))/10.0, (float(rand() % 100)) / 10.0, (float(rand() % 100)) / 10.0);


	int spot_poz = glGetUniformLocation(shader->program, "spot");
	glUniform1i(spot_poz, 1);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, sea->colorR, sea->colorG, sea->colorB);

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}



void Game::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Game::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){
	plane->update(-(deltaY)/720.0);
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
