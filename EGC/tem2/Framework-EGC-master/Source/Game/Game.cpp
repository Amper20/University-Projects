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
#include "Live.h"

using namespace std;

Game::Game(){
	
	camera = new Laborator::Camera();
	camera1 = new Laborator::Camera();

	camera1->Set(glm::vec3(0.0069, 3, 1.2), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	camera1->up = glm::vec3(0.013, 0.95, -0.32);
	camera1->forward = glm::vec3(0.04, -0.32, -0.95);
	camera1->right = glm::vec3(1, 0, 0.042);

	camera->Set(glm::vec3(1.93547, 2.57825, -0.0298535), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	camera->up = glm::vec3(-0.274446, 0.961524, -0.0123024);
	camera->forward = glm::vec3(-0.960559, -0.274722, -0.0430583);
	camera->right = glm::vec3(0.0447814, 0, -0.998997);
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	sea = new Sea();
	
	plane = new Plane();

	for (int i = 0; i < cloudCnt; i++) 
		cloud.push_back(new Cloud());
	
	for (int i = 0; i < fuelCnt; i++)
		fuel.push_back(new Fuel());

	for (int i = 0; i < live; i++)
		lives.push_back(new Live());

	for (int i = 0; i < fuelCnt; i++)
		obstacle.push_back(new Obstacle());
	
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
	planeData = plane->getFuel();
	RenderMesh(get<0>(planeData), get<2>(planeData), get<1>(planeData), plane->bodyColor);

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

	for (int i = 0; i < lives.size(); i++) {
		tuple<Mesh*, glm::mat4, Shader*, glm::vec3> liveData = lives[i]->getMesh((float)i*0.2);
		RenderMesh(get<0>(liveData), get<2>(liveData), get<1>(liveData), get<3>(liveData));
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
			lives[live]->flaged = true;
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
	gameTime += deltaTimeSeconds;
	if (((int)gameTime)%10 == 0) {
		speed += 0.005f;
	}
	for (int i = 0; i < lives.size(); i++) {
		lives[i]->Update(deltaTimeSeconds);
		if (i < live) {
			lives[i]->flaged = false;
		}
	}

	if (live > 0 && plane->fuel > 0) {

		deltaTimeSeconds *= speed;
		
		if (plane->fuel > 100.0) 
			plane->fuel = 100;
		plane->fuel -= deltaTimeSeconds * 7;

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
		if (remove_poz != -1 && fuel.size()) {
			fuel.erase(fuel.begin() + remove_poz);
			fuel.push_back(new Fuel());
		}

		remove_poz = -1;
		for (int i = 0; i < obstacle.size(); i++) {
			obstacle[i]->Update(deltaTimeSeconds);
			if (obstacle[i]->flaged > 50)
				remove_poz = i;
		}
		if (remove_poz != -1 && obstacle.size()) {
			obstacle.erase(obstacle.begin() + remove_poz);
			obstacle.push_back(new Obstacle());
		}
	}
	else {
		plane->dead = 1;
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
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			(view ? camera : camera1)->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			(view ? camera : camera1)->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			(view ? camera : camera1)->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			(view ? camera : camera1)->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			(view ? camera : camera1)->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			(view ? camera : camera1)->TranslateUpword(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_O)) {
			orthoLeft = -8.0f;
			orthoRight = 8.0f;
			orthoUp = 4.5f;
			orthoDown = -4.5;
			projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
			isOrtho = true;
		}

		// increase height
		if (window->KeyHold(GLFW_KEY_UP) && isOrtho) {
			orthoUp += deltaTime;
			orthoDown -= deltaTime;
			projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
		}

		// increase height
		if (window->KeyHold(GLFW_KEY_DOWN) && isOrtho) {
			orthoUp -= deltaTime;
			orthoDown += deltaTime;
			projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
		}

		// increase length
		if (window->KeyHold(GLFW_KEY_RIGHT) && isOrtho) {
			orthoRight += deltaTime;
			orthoLeft -= deltaTime;
			projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
		}

		// decrease length
		if (window->KeyHold(GLFW_KEY_LEFT) && isOrtho) {
			orthoRight -= deltaTime;
			orthoLeft += deltaTime;
			projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
		}

		// inversare FOV
		if (window->KeyHold(GLFW_KEY_K)) {
			projectionMatrix = glm::perspective(-90.f, -2.f, 2.f, -200.0f);
			isOrtho = false;
		}
		// FOV normal
		if (window->KeyHold(GLFW_KEY_P))
		{
			projectionMatrix = glm::perspective(90.f, 2.f, 2.f, 200.0f);
			isOrtho = false;
		}
	}
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

	glm::vec3 eyePosition =  (view ? camera : camera1)->position;//GetSceneCamera()->transform->GetWorldPosition();
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

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr((view ? camera : camera1)->GetViewMatrix()));

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

	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr((view ? camera : camera1)->GetViewMatrix()));

	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}



void Game::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C) {
		view = !view;
	}
	if (key == GLFW_KEY_R) {
		plane->fuel = 100.0f;
		live = 3;
		plane->yPoz = 0;
		plane->dead = 0;
		speed = 1.0f;
	}
}

void Game::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){
	plane->update(-(deltaY)/720.0);
	camera1->position = camera1->position + glm::vec3(0, -(deltaY) / 720.0, 0);
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			(view ? camera : camera1)->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
			(view ? camera : camera1)->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			(view ? camera : camera1)->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
			(view ? camera : camera1)->RotateThirdPerson_OY(-2 * sensivityOY * deltaX);
		}

	}
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
