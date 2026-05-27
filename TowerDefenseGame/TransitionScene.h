#pragma once
#include "Scene.h"

using namespace sf;

class Game;

class TransitionScene : public Scene
{
public:
	TransitionScene(RenderWindow& renderWindow, Game* game = nullptr, int levelNumber = 1);
	Scenes run() override;
	bool init() override;

private:
	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	View view;
	Game* game = nullptr;
	int levelNumber = 1;

	Font font;
	Text* message = nullptr;

	float transitionTimer = 0.0f;
	static constexpr float TRANSITION_TIME = 3.0f;

	Scenes nextScene;
};
