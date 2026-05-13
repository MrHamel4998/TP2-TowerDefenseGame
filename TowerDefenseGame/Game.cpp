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
	Scene::Scenes sceneSelector = Scene::Scenes::Level2;
	Scene* activeScene = nullptr; //Pointeur de la super-classe, peut pointer sur n'importe quelle scène

	//Les variables de passage d'information entre scènes devraient être déclarés ici

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
			//Les deux attributs sont récessaire et passés par référence
			activeScene = new TitleScene(renderWindow);
			break;
		case Scene::Scenes::Transition:
			//Les deux attributs sont récessaire et passés par référence
			activeScene = new TransitionScene(renderWindow);
			break;
		case Scene::Scenes::Level1:
			//Les deux attributs sont récessaire et passés par référence
			activeScene = new GameScene(renderWindow);
			break;
		case Scene::Scenes::Level2:
			//Les deux attributs sont récessaire et passés par référence
			activeScene = new GameScene2(renderWindow);
			break;
		case Scene::Scenes::End:
			activeScene = new EndScene(renderWindow);
			break;
		}
		
		if (activeScene->init()) //Si l'initilisation s'est bien passé, on entre dans ce bloc
		{
			//Run est la boucle de jeu de la scène
			//À la fin de cette méthode, elle retourne la scène
			//Laquelle on transition
			sceneSelector = activeScene->run();

			//À la fin d'une scène, s'il y a des sauvegardes à faire
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

		//Nécessaire tout ce qui est crée avec new doit être effacé.
		delete activeScene;
		activeScene = nullptr;
	}
}