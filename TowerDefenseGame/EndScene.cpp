#include "EndScene.h"
#include "ContentPipeline.h"

EndScene::EndScene(RenderWindow& renderWindow) : Scene(renderWindow)
{
	view = renderWindow.getDefaultView();
}

Scene::Scenes EndScene::run()
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

bool EndScene::init()
{
	//--Image et musique-------------------------------------------------------------------------//
	if (isVictory)
	{
		if (!themeSong.openFromFile("Ressources\\Sounds\\Music\\VictoryTheme.ogg")) return false;
		endGameScreen = new Sprite(ContentPipeline::getInstance().getVictoryScreenTexture());
	}
	else
	{
		if (!themeSong.openFromFile("Ressources\\Sounds\\Music\\EndGameTheme.ogg")) return false;
		endGameScreen = new Sprite(ContentPipeline::getInstance().getGameOverScreenTexture());
	}

	//--Préparation de la font-------------------------------------------------------------------------//
	instructions[0] = new Text(ContentPipeline::getInstance().getComiciFont(), "", 50U);
	instructions[0]->setOutlineThickness(4.0f);

	if (isVictory)
	{
		instructions[0]->setOutlineColor(Color(232, 161, 10, 255));
		instructions[0]->setFillColor(Color::Yellow);
	}
	else
	{
		instructions[0]->setOutlineColor(Color(93, 31, 4, 255));
		instructions[0]->setFillColor(Color::Red);
	}

	//Utilisation du constructeur de copie
	for (int i = 1; i < INSTRUCTIONS_NUMBER; i++) instructions[i] = new Text(*instructions[0]);

	instructions[0]->setString("Score - 0 (Wave - 1)");
	instructions[1]->setString("HighScore - 0 (Wave - 1");
	instructions[2]->setString("Press Enter to go back to title screen");
	instructions[3]->setString("Press Escape to exit");

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
		instructions[i]->setOrigin({ instructions[i]->getGlobalBounds().size.x / 2.0f, instructions[i]->getGlobalBounds().size.y / 2.0f });

	if (isVictory)
	{
		instructions[0]->setPosition({ SCREEN_WIDTH / 2.0f, 70.0f });
		instructions[1]->setPosition({ SCREEN_WIDTH / 2.0f, 170.0f });
		instructions[2]->setPosition({ SCREEN_WIDTH / 2.0f, 850.0f });
		instructions[3]->setPosition({ SCREEN_WIDTH / 2.0f, 950.0f });
	}
	else
	{
		instructions[0]->setPosition({ SCREEN_WIDTH / 2.0f, 160.0f });
		instructions[1]->setPosition({ SCREEN_WIDTH / 2.0f, 310.0f });
		instructions[2]->setPosition({ SCREEN_WIDTH / 2.0f, 770.0f });
		instructions[3]->setPosition({ SCREEN_WIDTH / 2.0f, 920.0f });
	}

	themeSong.setLooping(true);
	themeSong.play();

	return true;
}

void EndScene::getInputs()
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

void EndScene::update()
{
}

void EndScene::draw()
{
	renderWindow.clear();
	renderWindow.draw(*endGameScreen);

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++) 
		renderWindow.draw(*instructions[i]);

	renderWindow.display();
}

bool EndScene::unload()
{
	if (endGameScreen != nullptr) delete endGameScreen;

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
		if (instructions[i] != nullptr) delete instructions[i];

	return true;
}