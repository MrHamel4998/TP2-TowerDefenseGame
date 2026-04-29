#pragma once
#include "Scene.h"

using namespace sf;

class TitleScene : public Scene
{
public:
	TitleScene(RenderWindow& renderWindow);
	Scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;

	Sprite* titleScreen = nullptr;
	Text* instructions = nullptr;
	Music themeSong;
};