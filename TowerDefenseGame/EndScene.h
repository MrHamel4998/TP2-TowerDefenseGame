#pragma once
#include "Scene.h"

using namespace sf;

class Game;

class EndScene : public Scene
{
public:
	EndScene(RenderWindow& renderWindow, Game* game = nullptr, bool isVictory = false);
	Scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;
	Game* game = nullptr;

	Sprite* endGameScreen = nullptr;
	static const int INSTRUCTIONS_NUMBER = 4;
	Text* instructions[INSTRUCTIONS_NUMBER] = {};
	Music themeSong;

	bool isVictory = false;
};