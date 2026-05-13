#include "TitleScene.h"
#include "ContentPipeline.h"

TitleScene::TitleScene(RenderWindow& renderWindow) : Scene(renderWindow)
{
	view = renderWindow.getDefaultView();
}

Scene::Scenes TitleScene::run()
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

bool TitleScene::init()
{
	if (!themeSong.openFromFile("Ressources\\Sounds\\Music\\TitleTheme.ogg")) return false;

	titleScreen = new Sprite(ContentPipeline::getInstance().getTitleScreenTexture());

	instructions =  new Text(ContentPipeline::getInstance().getComiciFont(), "Press Enter to play", 70U);
	instructions->setOutlineColor(Color::Black);
	instructions->setFillColor(Color(186, 62, 8, 255));
	instructions->setOutlineThickness(4.0f);
	instructions->setOrigin({ instructions->getGlobalBounds().size.x / 2.0f, instructions->getGlobalBounds().size.y / 2.0f });
	instructions->setPosition({ SCREEN_WIDTH / 2.0f, 550.0f });

	themeSong.setLooping(true);
	themeSong.play();
	return true;
}

void TitleScene::getInputs()
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

void TitleScene::update()
{
}

void TitleScene::draw()
{
	renderWindow.clear();
	renderWindow.draw(*titleScreen);
	renderWindow.draw(*instructions);
	renderWindow.display();
}

bool TitleScene::unload()
{
	if (titleScreen != nullptr) delete titleScreen;
	if (instructions != nullptr) delete instructions;

	return true;
}