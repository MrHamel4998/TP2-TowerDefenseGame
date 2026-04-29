#include "TransitionScene.h"
#include "ContentPipeline.h"

TransitionScene::TransitionScene(RenderWindow& renderWindow) : Scene(renderWindow)
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
	message = new Text(ContentPipeline::getInstance().getComiciFont(), "Wave X", 80U);
	//message->setString("Minons of the Dark Lord march on our lands.\n\n             Defend the King's Tower!\n\n                           Wave 1");
	//message->setCharacterSize(60U);

	message->setFillColor(Color::White);
	message->setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
	message->setOrigin({ message->getGlobalBounds().size.x / 2.0f, message->getGlobalBounds().size.y / 2.0f });
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

}

void TransitionScene::draw()
{
	renderWindow.clear();
	renderWindow.draw(*message);
	renderWindow.display();
}

bool TransitionScene::unload()
{
	if (message != nullptr) 
		delete message;
	return true;
}