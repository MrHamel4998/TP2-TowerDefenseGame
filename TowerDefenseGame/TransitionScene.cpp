#include "TransitionScene.h"
#include "Game.h"
#include "ContentPipeline.h"

TransitionScene::TransitionScene(RenderWindow& renderWindow, Game* game, int levelNumber) 
	: Scene(renderWindow), game(game), levelNumber(levelNumber)
{
	view = renderWindow.getDefaultView();
}

Scene::Scenes TransitionScene::run()
{
	while (isRunning)
	{
		calculateDeltaTime();
		getInputs();
		update();
		draw();
	}

	if (!unload()) return Scenes::Fail;

	return transitionToScene;
}

bool TransitionScene::init()
{
	isRunning = true;
	transitionToScene = Scene::Scenes::Fail;

	String messageStr = "Wave " + std::to_string(levelNumber + 1);
	nextScene = (levelNumber % 2 == 0) ? Scene::Scenes::Level1 : Scene::Scenes::Level2;

	message = new Text(ContentPipeline::getInstance().getComiciFont(), messageStr, 50U);
	message->setFillColor(Color::White);
	message->setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
	message->setOrigin({ message->getGlobalBounds().size.x / 2.0f, message->getGlobalBounds().size.y / 2.0f });

	transitionTimer = 0.0f;

	return true;
}

void TransitionScene::getInputs()
{
	while (const optional event = renderWindow.pollEvent())
	{
		//x sur la fenêtre
		if (event->is<Event::Closed>())
		{
			isRunning = false;
			transitionToScene = Scene::Scenes::Exit;
		}
	}
}

void TransitionScene::update()
{
	transitionTimer += deltaTime;

	if (transitionTimer >= TRANSITION_TIME)
	{
		isRunning = false;
		transitionToScene = nextScene;
	}
}

void TransitionScene::draw()
{
	renderWindow.clear();
	if (message != nullptr)
	{
		renderWindow.draw(*message);
	}
	renderWindow.display();
}

bool TransitionScene::unload()
{
	if (message != nullptr) 
		delete message;
	return true;
}
