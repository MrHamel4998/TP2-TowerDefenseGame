#include "Game.h"
#include "ContentPipeline.h"
#include "TitleScene.h"
#include "TransitionScene.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "EndScene.h"


Game::Game()
{
	//On place dans le contructeur ce qui permet à la game elle-même de fonctionner

	renderWindow.create(VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }, 32), "Tower Defense Game");// , Style::None, State::Fullscreen);

	//Synchonisation coordonnée à l'écran!  Normalement 60 frames par secondes. À faire absolument
	//renderWindow.setVerticalSyncEnabled(true);  //De plus en plus d'écrans sont rafraichis à plus de 60 frames par seconde, alors attention.
	//renderWindow.setFramerateLimit(60);  //Équivalent... normalement, mais pas toujours. À utiliser si la synchonisation de l'écran fonctionne mal.
	//https://www.sfml-dev.org/tutorials/3.0/window/window/#controlling-the-framerate

	renderWindow.setKeyRepeatEnabled(false);
	srand((int)time(0));

	//Nouveau: toujours la même chose pour avoir un icon dans l'explorateur Windows
	if (icon.loadFromFile("Ressources\\Sprites\\Misc\\Icon.png"))
		renderWindow.setIcon({ icon.getSize().x, icon.getSize().y }, icon.getPixelsPtr());
}

int Game::run()
{
	if (!ContentPipeline::getInstance().loadContent()) return EXIT_FAILURE;
	//Un enum et un pointeur de scene pour faire la manipulation de scène
	Scene::Scenes sceneSelector = Scene::Scenes::Title;
	Scene* activeScene = nullptr; //Pointeur de la super-classe, peut pointer sur n'importe quelle scène

	int transitionLevel = 0; // 0 = vers Level1, 1 = vers Level2, etc.
	int maxWaves = shortMode ? 2 : 10; // 2 vagues en mode court, sinon 10 vagues

	while (true)
	{
		//Seules conditions de sortie de toute l'app, une pour les sorties normales, une pour les erreurs
		//On est au seul point de sortie
		if (sceneSelector == Scene::Scenes::Exit)
			return EXIT_SUCCESS;
		if (sceneSelector == Scene::Scenes::Fail)
			return EXIT_FAILURE;


		//Vous allez ajouter d'autre scènes, alors elles devront
		//être ajoutées ici
		switch (sceneSelector)
		{
		case Scene::Scenes::Title:
			activeScene = new TitleScene(renderWindow, this);
			break;
		case Scene::Scenes::Transition:
			activeScene = new TransitionScene(renderWindow, this, transitionLevel);
			break;
		case Scene::Scenes::Level1:
			activeScene = new GameScene(renderWindow, this);
			break;
		case Scene::Scenes::Level2:
			activeScene = new GameScene2(renderWindow, this);
			break;
		case Scene::Scenes::End:
			activeScene = new EndScene(renderWindow, victory);
			break;
		}

		Scene::Scenes ranScene = sceneSelector;

		if (activeScene->init()) //Si l'initilisation s'est bien passé, on entre dans ce bloc
		{
			//Run est la boucle de jeu de la scène
			//À la fin de cette méthode, elle retourne la scène
			//Laquelle on transition
			sceneSelector = activeScene->run();

			//C'est possible de les faire là.
			/*SceneGame* tempScene = dynamic_cast<SceneGame*>(activeScene);
			if (tempScene != nullptr)//Donc si le cast a réussi.
			{

			}*/			
		}
		else //Si l'initialisation rate (exemple: pour assets mal chargés), on fail et on nettoie ce qui est à nettoyer
		{
			sceneSelector = Scene::Scenes::Fail;
			//clean-up éventuel à faire pour s'assurer 
			//de ne pas avoir de leak (malgré l'échec)
		}		

		delete activeScene;
		activeScene = nullptr;

		maxWaves = shortMode ? 2 : 10;

		if (sceneSelector == Scene::Scenes::Transition)
		{

			if (ranScene != Scene::Scenes::Title)
			{
				nextWave();
			}
			transitionLevel = currentWave - 1;
		}

		if (currentWave > maxWaves)
		{
			setVictory(true);
			sceneSelector = Scene::Scenes::End;
		}

		if (isGameOver() && sceneSelector == Scene::Scenes::Transition)
		{
			sceneSelector = Scene::Scenes::End;
		}
	}
}

bool Game::isShortMode() const 
{ 
	return shortMode;
}

void Game::setShortMode(bool value) 
{ 
	shortMode = value; 
}

int Game::getCurrentWave() const 
{ 
	return currentWave;
}

int Game::getMaxWaves() const
{
	return shortMode ? 2 : 10;
}

bool Game::isVictory() const 
{ 
	return victory;
}

void Game::setVictory(bool value) 
{ 
	victory = value; 
}

bool Game::isGameOver() const 
{ 
	return gameOver;
}

void Game::setGameOver(bool value) 
{ 
	gameOver = value; 
}

void Game::nextWave() 
{ 
	currentWave++; 
}

void Game::reset()
{
	shortMode = false;
	victory = false;
	gameOver = false;
	currentWave = 1;
}
