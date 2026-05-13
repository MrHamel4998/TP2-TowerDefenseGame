#pragma once
#include "Scene.h"

using namespace sf;

class EndScene : public Scene
{
public:
	EndScene(RenderWindow& renderWindow);
	Scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;

	Sprite* endGameScreen = nullptr;
	static const int INSTRUCTIONS_NUMBER = 4;
	Text* instructions[INSTRUCTIONS_NUMBER] = {};
	Music themeSong;

	bool isVictory = false;
};