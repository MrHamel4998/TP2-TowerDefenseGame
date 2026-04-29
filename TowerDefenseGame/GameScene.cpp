#include "GameScene.h"
#include "ContentPipeline.h"

GameScene::GameScene(RenderWindow& renderWindow) : Scene(renderWindow)
{
	view = renderWindow.getDefaultView();
}

Scene::Scenes GameScene::run()
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

bool GameScene::init()
{	
	map = new Sprite(ContentPipeline::getInstance().getMapTexture(Maps::Map1));

	hud.hudInit(ContentPipeline::getInstance().getHudmaskTexture(), ContentPipeline::getInstance().getComiciFont());

	return true;
}

void GameScene::getInputs()
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

void GameScene::update()
{

}

void GameScene::draw()
{
	//Toujours important d'effacer l'écran précédent
	renderWindow.clear();
	renderWindow.draw(*map);

	hud.draw(renderWindow);
	renderWindow.display();
}

bool GameScene::unload()
{
	if (map != nullptr) delete map;
	return true;
}