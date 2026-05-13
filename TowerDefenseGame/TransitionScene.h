#pragma once
#include "Scene.h"

using namespace sf;

class TransitionScene : public Scene
{
public:
	TransitionScene(RenderWindow& renderWindow);
	Scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;

	Font font;
	Text* message = nullptr;
};