#include "GameScene.h"
#include "ContentPipeline.h"
#include <iostream>

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

	for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
	{
		towers[i] = nullptr;
	}

	for (int i = 0; i < NUM_PROJECTILES; i++) 
	{
		projectiles[i] = nullptr;
	}

	Vector2f emplacementPositions[NUM_TOWERS_EMPLACEMENT] = {
	Vector2f(470, 170), Vector2f(770, 250), Vector2f(440, 370),
	Vector2f(650, 520), Vector2f(120, 650), Vector2f(470, 700),
	Vector2f(850, 710), Vector2f(660, 950)
	};

	for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
	{
		towersEmplacement[i] = new TowerEmplacement();
		towersEmplacement[i]->init();
		towersEmplacement[i]->setPosition(emplacementPositions[i]);
		towersEmplacement[i]->activate();
		Subject::addObserver(towersEmplacement[i]);
	}

	int towerIndex = 0;

	kingTower = Tower::createKingTower();
	kingTower->setPosition(Vector2f(1138, 600));
	kingTower->activate();

	//Création des tours d'archers
	for (int i = 0; i < NUM_TOWERS; i++)
	{
		towers[towerIndex] = ShootingTower::create(TowersType::ARCHER);
		towerIndex++;
	}

	//Création des tours de mage
	for (int i = 0; i < NUM_TOWERS; i++)
	{
		towers[towerIndex] = ShootingTower::create(TowersType::MAGE);
		towerIndex++;
	}


	Subject::addObserver(this);
	sacredLight.init();
	plague.init();

	isRunning = true;
	demonsKilled = 0;

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

		// Sélection Sacred Light
		if (Keyboard::isKeyPressed(Keyboard::Key::V))
		{
			inputs.sacredLightSelected = true;
			inputs.plagueSelected = false;
			inputs.archerTowerSelected = false;
			inputs.mageTowerSelected = false;
			std::cout << "Sacred Light selected" << std::endl;
		}

		// Sélection Plague
		if (Keyboard::isKeyPressed(Keyboard::Key::C))
		{
			inputs.plagueSelected = true;
			inputs.sacredLightSelected = false;
			inputs.mageTowerSelected = false;
			inputs.archerTowerSelected = false;
			std::cout << "Plague selected" << std::endl;
		}

		// Sélection Tour de Mage
		if (Keyboard::isKeyPressed(Keyboard::Key::X))
		{
			inputs.sacredLightSelected = false;
			inputs.plagueSelected = false;
			inputs.archerTowerSelected = false;
			inputs.mageTowerSelected = true;
			std::cout << "Mage Tower selected" << std::endl;
		}

		// Sélection Tour d'Archer
		if (Keyboard::isKeyPressed(Keyboard::Key::Z))
		{
			inputs.sacredLightSelected = false;
			inputs.plagueSelected = false;
			inputs.archerTowerSelected = true;
			inputs.mageTowerSelected = false;
			std::cout << "Archer Tower selected" << std::endl;
		}

		inputs.leftMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);
	}
}

void GameScene::update()
{
	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		if (demons[i] != nullptr && demons[i]->isActive())
		{
			demons[i]->update(deltaTime);
			demons[i]->shoot(deltaTime, towers, kingTower, NUM_TOWERS * NUM_TOWERS_TYPE, projectiles, NUM_PROJECTILES, currentWaveNumber);
		}
	}

	if (kingTower != nullptr && kingTower->isActive())
	{
		kingTower->update(deltaTime);
	}

	for (int i = 0; i < NUM_TOWERS * NUM_TOWERS_TYPE; i++)
	{
		if (towers[i] != nullptr && towers[i]->isActive())
		{
			towers[i]->shoot(deltaTime, demons, NUM_DEMONS_TOTAL, projectiles, NUM_PROJECTILES, currentWaveNumber);
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
			spawnTimer = 0.0f;
			nextSpawnTime = 1.0f + static_cast<float>(rand()) / RAND_MAX * (3.0f - 1.0f);
			demonsSpawned++;
		}
		// Sinon, on réinitialise le timer pour réessayer au prochain intervalle
		else
		{
			spawnTimer = 0.f;
		}
	}

	handleBuilding();
	sacredLight.update(deltaTime);
	plague.update(deltaTime);

	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		if (projectiles[i] != nullptr && projectiles[i]->isActive())
		{
			projectiles[i]->update(deltaTime);

			if (projectiles[i]->hasReachedTarget())
			{
				if (Tower* targetTower = dynamic_cast<Tower*>(projectiles[i]->getTarget()))
				{
					if (targetTower->isActive())
					{
						targetTower->takeDamage(projectiles[i]->getDamage());
					}
				}

				projectiles[i]->consumeImpact();
			}
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

	if (kingTower != nullptr && kingTower->isActive())
	{
		kingTower->draw(renderWindow);
	}

	for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
	{
		if (towersEmplacement[i] != nullptr && towersEmplacement[i]->isActive())
			towersEmplacement[i]->draw(renderWindow);
	}

	for (int i = 0; i < (NUM_TOWERS * NUM_TOWERS_TYPE); i++)
	{
		if (towers[i] != nullptr && towers[i]->isActive())
		{
			towers[i]->draw(renderWindow);
		}
	}
	
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		if (projectiles[i] != nullptr && projectiles[i]->isActive())
		{
			projectiles[i]->draw(renderWindow);
		}
	}
	handleSpells();
	sacredLight.draw(renderWindow);
	plague.draw(renderWindow);

	hud.draw(renderWindow);
	drawWaypoints();
	renderWindow.display();
}

bool GameScene::unload()
{
	Subject::removeAllObservers();

	if (map != nullptr) delete map;

	for (int i = 0; i < NUM_WAYPOINTS; i++)
	{
		delete waypoints[i];
	}

	if (kingTower != nullptr)
	{
		delete kingTower;
	}

	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		if (demons[i] != nullptr)
		{
			delete demons[i];
		}
	}

	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		if (projectiles[i] != nullptr)
		{
			delete projectiles[i];
		}
	}

	for (int i = 0; i < (NUM_TOWERS * NUM_TOWERS_TYPE); i++)
	{
		if (towers[i] != nullptr)
		{
			delete towers[i];
		}
	}

	for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
	{
		if (towersEmplacement[i] != nullptr)
			delete towersEmplacement[i];
	}

	return true;
}

void GameScene::notify(Subject* subject, EventType eventType)
{
	if (eventType == EventType::DemonKilled)
	{
		demonsKilled++;

		if (demonsKilled >= DEMON_TO_SPAWN)
		{
			isRunning = false;
			transitionToScene = Scene::Scenes::Level2;
		}
	}
	else if (eventType == EventType::DemonDamageTaken)
	{
		// CH: Mettre à jour les points, le mana, etc.
		// On peut call le hud ou la gameScene
	}
	else if (eventType == EventType::TowerActivated)
	{
		// CH: Quand une tour est placée (Activée), on retire du mana, etc.
	}
	else if (eventType == EventType::TowerDeactivated)
	{
		// CH: Quand une tour est détruite (Désactivée), on peut réinitialiser l'emplacement, etc.
	}
	else if (eventType == EventType::WaveFinished)
	{
		// CH: La vague est terminée
		// A voir, car on le fait déja dans le notify de DemonKilled
	}
}

void GameScene::drawWaypoints()
{
	if (!inputs.showWaypoints)
	{
		return;
	}
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

void GameScene::handleSpells()
{
	if (!inputs.leftMousePressed)
	{
		return;
	}

	Vector2f mouseWorldPos = renderWindow.mapPixelToCoords(Mouse::getPosition(renderWindow));

	targetCount = 0;
	for (Demon* demon : demons)
	{
		if (demon != nullptr && demon->isActive())
		{
			if (targetCount >= 100) break;
			targets[targetCount] = demon;
			targetCount++;
		}
	}

	for (Tower* tower : towers)
	{
		if (tower != nullptr && tower->isActive())
		{
			if (targetCount >= 100) break;
			targets[targetCount] = tower;
			targetCount++;
		}
	}

	// Sacred Light
	if (inputs.sacredLightSelected && !sacredLight.isActive())
	{
		sacredLight.cast(
			mouseWorldPos,
			targets,
			targetCount
		);
	}

	// Plague
	if (inputs.plagueSelected && !plague.isActive())
	{
		plague.cast(
			mouseWorldPos,
			targets,
			targetCount
		);
	}

	inputs.leftMousePressed = false;
}

void GameScene::handleBuilding()
{
	if (!inputs.leftMousePressed) return;
	if (!inputs.archerTowerSelected && !inputs.mageTowerSelected) return;

	Vector2f mouseWorldPos = renderWindow.mapPixelToCoords(Mouse::getPosition(renderWindow));

	//Recherche de l'emplacement cliqué
	TowerEmplacement* selectedEmplacement = nullptr;
	for (TowerEmplacement* towerEmplacement : towersEmplacement)
	{
		if (!towerEmplacement->isOccupied())
		{
			float dist = (mouseWorldPos - towerEmplacement->getPosition()).length();
			if (dist < 50.f)
			{
				selectedEmplacement = towerEmplacement;
				break;
			}
		}
	}

	if (selectedEmplacement == nullptr) return;

	//Recherche d'une tour inactive du bon type
	TowersType desiredType = inputs.archerTowerSelected ? TowersType::ARCHER : TowersType::MAGE;
	Tower* newTower = nullptr;
	for (Tower* tower : towers)
	{
		if (tower != nullptr && !tower->isActive() && tower->getType() == desiredType)
		{
			newTower = tower;
			break;
		}
	}

	//Place la tour
	if (newTower == nullptr) return;
	newTower->setPosition(selectedEmplacement->getPosition());
	newTower->setLifePoints(250);
	newTower->activate();
	selectedEmplacement->placeTower(newTower);
}