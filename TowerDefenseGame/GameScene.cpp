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

	waypoints[0] = new Waypoint(Vector2f(610, 8));
	waypoints[1] = new Waypoint(Vector2f(630, 222));
	waypoints[2] = new Waypoint(Vector2f(595, 444));
	waypoints[3] = new Waypoint(Vector2f(478, 514));
	waypoints[4] = new Waypoint(Vector2f(320, 558));
	waypoints[5] = new Waypoint(Vector2f(260, 620));
	waypoints[6] = new Waypoint(Vector2f(280, 720));
	waypoints[7] = new Waypoint(Vector2f(348, 812));
	waypoints[8] = new Waypoint(Vector2f(526, 862));
	waypoints[9] = new Waypoint(Vector2f(720, 830));
	waypoints[10] = new Waypoint(Vector2f(968, 850));
	waypoints[11] = new Waypoint(Vector2f(1110, 682));

	for (int i = 0; i < NUM_WAYPOINTS - 1; i++)
	{
		waypoints[i]->setNextWaypoint(waypoints[i + 1]);
	}

	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		demons[i] = new Demon();
	}

	return true;
}

void GameScene::getInputs()
{
	while (const optional event = renderWindow.pollEvent())
	{
		// x sur la fenêtre
		if (event->is<Event::Closed>())
		{
			isRunning = false;
			transitionToScene = Scene::Scenes::Exit;
		}
		inputs.showWaypoints = Keyboard::isKeyPressed(Keyboard::Key::W);
	}
}

void GameScene::update()
{
	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		if (demons[i] != nullptr && demons[i]->isActive())
		{
			demons[i]->update(deltaTime);
		}
	}

	spawnTimer += deltaTime;

	if (spawnTimer >= nextSpawnTime && demonsSpawned < DEMON_TO_SPAWN) {
		// Chercher un slot libre dans le tableau
		int freeSlot = -1;
		for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
		{
			if (demons[i] == nullptr || !demons[i]->isActive())
			{
				freeSlot = i;
				break;
			}
		}

		// Si un slot est libre, spawner le démon
		if (freeSlot != -1)
		{
			if (demons[freeSlot] == nullptr)
			{
				demons[freeSlot] = new Demon();
			}
			demons[freeSlot]->spawn(DEMON_SPAWN_POSITION, waypoints[0], currentWaveNumber);
			spawnTimer = 0.f;
			nextSpawnTime = 1.f + static_cast<float>(rand()) / RAND_MAX * (3.0f - 1.0f);
			demonsSpawned++;
		}
		// Sinon, on réinitialise le timer pour réessayer au prochain intervalle
		else
		{
			spawnTimer = 0.f;
		}
	}
}

void GameScene::draw()
{
	// Toujours important d'effacer l'écran précédent
	renderWindow.clear();
	renderWindow.draw(*map);

	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		if (demons[i] != nullptr && demons[i]->isActive())
		{
			demons[i]->draw(renderWindow);
		}
	}

	hud.draw(renderWindow);
	drawWaypoints();
	renderWindow.display();
}

bool GameScene::unload()
{
	if (map != nullptr) delete map;

	for (int i = 0; i < NUM_WAYPOINTS; i++)
	{
		delete waypoints[i];
	}
	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		if (demons[i] != nullptr)
		{
			delete demons[i];
		}
	}

	return true;
}

void GameScene::drawWaypoints()
{
	if (!inputs.showWaypoints)
		return;
	if (inputs.showWaypoints)
	{
		for (int i = 0; i < NUM_WAYPOINTS - 1; i++)
		{
			CircleShape waypointShape(10.0f);
			waypointShape.setFillColor(Color::Red);
			waypointShape.setPosition(waypoints[i]->getNextWaypoint()->getPosition());
			renderWindow.draw(waypointShape);
		}
	}
}
