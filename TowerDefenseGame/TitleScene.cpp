#include "TitleScene.h"
#include "Game.h"
#include "ContentPipeline.h"

TitleScene::TitleScene(RenderWindow& renderWindow, Game* game) : Scene(renderWindow), game(game)
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
	inputs.reset();
	while (const optional event = renderWindow.pollEvent())
	{
		//x sur la fenêtre
		if (event->is<Event::Closed>())
		{
			inputs.escapePressed = true;
			isRunning = false;
			transitionToScene = Scene::Scenes::Exit;
		}
		else if (const Event::KeyPressed* keyPressed = event->getIf<Event::KeyPressed>())
		{
			switch (keyPressed->scancode)
			{
				case Keyboard::Scan::Enter:
					inputs.enterPressed = true;
					isRunning = false;
					transitionToScene = Scene::Scenes::Transition;
					break;
				case Keyboard::Scan::S:
					inputs.sPressed = true;
					if (game != nullptr)
					{
						game->setShortMode(true);
					}
					break;
				case Keyboard::Scan::Escape:
					inputs.escapePressed = true;
					isRunning = false;
					transitionToScene = Scene::Scenes::Exit;
					break;
				default:
					break;
			}
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
