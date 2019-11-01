#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <Bird.h>
#include <Obstacle.h>

class Game : public SimpleScene{

public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void Render();
	void UpdateObjects(float deltaTimeSeconds);
	void CheckCollision();
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void updateScore(float bonus);

protected:
	glm::mat3 modelMatrix;
	Bird bird;
	Obstacle obstacle;
	float g = -200.0;
	float translateX, translateY, scaleX, scaleY, angularStep;
	float score;
	bool gameOver = false;

};
