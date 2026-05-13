#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using std::optional;

/// <summary>
/// Classe abstraite avec aucune méthode concrétisée, sauf le constructeur et le destructeur
/// (qui restent très simples)  Un .h suffit donc.
/// </summary>
class Scene
{

public:
	//Quand vos ajouterez des scènes, ajouter un enum ici.  Exit et Fail sont des marqueurs de sortie
	//Game est très générique et est valable si on a une seule scène de jeu.
	//Sinon on pourrait parler de "Level1, Level2, ou encore Game_Scene1, Game_Scene2, etc.
	//Exit et Fail devraient toujours être présents.
	enum Scenes { Title, Level1, Level2, Transition, End, Exit, Fail };

	//après le : on fait appel au constructeur de la super classe
	//qui doit être aussi dans la liste d'initialisation
	Scene(RenderWindow& renderWindow) : renderWindow(renderWindow) {}

	virtual ~Scene() {}  //Pour que le destructeur de l'objet en mémoire soit apellé
	virtual Scenes run() = 0;
	virtual bool init() = 0;

protected:
	//La boucle de jeu est maintenant gérée par la scène
	void calculateDeltaTime() { deltaTime = clock.restart().asSeconds(); }
	virtual void getInputs() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool unload() = 0;

	Clock clock;
	float deltaTime = 0.0f;

	//Chaque scène aura une référence de la fenêtre portée par la game elle-même.
	RenderWindow& renderWindow;

	/// <summary>
	/// À la fin de chaque scène, on indique à Game quelle nouvelle scène elle doit charger.
	/// </summary>
	Scene::Scenes transitionToScene;
	bool isRunning;
};