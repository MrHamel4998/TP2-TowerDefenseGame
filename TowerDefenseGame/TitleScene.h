#pragma once
#include "Scene.h"
#include "Inputs.h"

using namespace sf;

class Game;

class TitleScene : public Scene
{
public:
	TitleScene(RenderWindow& renderWindow, Game* game = nullptr);
	Scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;
	Game* game = nullptr;
	Inputs inputs;

	Sprite* titleScreen = nullptr;
	Text* instructions = nullptr;
	Music themeSong;
};
