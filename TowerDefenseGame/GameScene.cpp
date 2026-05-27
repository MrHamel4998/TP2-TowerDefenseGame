#include "GameScene.h"
#include "ContentPipeline.h"
#include "Game.h"
#include "Constants.h"

GameScene::GameScene(RenderWindow& renderWindow, class Game* game) 
	: Scene(renderWindow), game(game)
{
	view = renderWindow.getDefaultView();
}

void GameScene::updateProjectiles()
{
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		Projectile* projectile = projectiles[i];
		if (projectile != nullptr && projectile->isActive())
		{
			projectile->update(deltaTime);

			if (projectile->hasReachedTarget())
			{
				if (Tower* targetTower = dynamic_cast<Tower*>(projectile->getTarget()))
				{
					if (targetTower->isActive())
					{
						targetTower->takeDamage(projectile->getDamage());
					}
				}
				else if (Demon* targetDemon = dynamic_cast<Demon*>(projectile->getTarget()))
				{
					if (targetDemon->isActive())
					{
						targetDemon->takeDamage(projectile->getDamage());
						if (game != nullptr) game->addScore(projectile->getDamage());
					}
				}

				projectile->consumeImpact();
			}
		}
	}
}

void GameScene::drawDemons()
{
	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		Demon* demon = demons[i];
		if (demon != nullptr && demon->isActive())
		{
			demon->draw(renderWindow);
		}
	}
}

void GameScene::drawProjectiles()
{
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		Projectile* projectile = projectiles[i];
		if (projectile != nullptr && projectile->isActive())
		{
			projectile->draw(renderWindow);
		}
	}
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
	inputs.reset();

	if (!music.openFromFile(std::filesystem::path(MUSIC_PATH_ARRAY[rand() % NBR_MUSIC])))
	{
		return false;
	}

	map = new Sprite(ContentPipeline::getInstance().getMapTexture(getMapId()));

	hud.hudInit(ContentPipeline::getInstance().getHudmaskTexture(), ContentPipeline::getInstance().getComiciFont());

	const Vector2f* waypointPositions = getWaypointPositions();
	waypointCount = getWaypointPositionsCount();
	for (int i = 0; i < waypointCount; i++)
	{
		waypoints[i] = new Waypoint(waypointPositions[i]);
	}

	configureWaypoints();

	for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
	{
		demons[i] = new Demon();
	}

	for (int i = 0; i < towerEmplacementCount; i++)
	{
		towers[i] = nullptr;
	}

	for (int i = 0; i < NUM_PROJECTILES; i++) 
	{
		projectiles[i] = nullptr;
	}

	const Vector2f* emplacementPositions = getTowerEmplacementPositions();
	towerEmplacementCount = getTowerEmplacementPositionsCount();

	for (int i = 0; i < towerEmplacementCount; i++)
	{
		towersEmplacement[i] = new TowerEmplacement();
		towersEmplacement[i]->init();
		towersEmplacement[i]->setPosition(emplacementPositions[i]);
		towersEmplacement[i]->activate();
		Subject::addObserver(towersEmplacement[i]);
	}

	int towerIndex = 0;

	kingTower = Tower::createKingTower();
	kingTower->setPosition(getKingTowerPosition());
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
	if (towerIndex > NUM_TOWERS * NUM_TOWERS_TYPE)
	{
		towerIndex = NUM_TOWERS * NUM_TOWERS_TYPE;
	}
	totalTowersCount = towerIndex;


	Subject::addObserver(this);
	sacredLight.init();
	plague.init();
	music.setLooping(true);
	music.play();

	isRunning = true;
	demonsKilled = 0;
	levelWon = false;
	gameOver = false;
	currentWaveNumber = (game != nullptr) ? game->getCurrentWave() : 1;

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

		if (const Event::KeyPressed* keyPressed = event->getIf<Event::KeyPressed>())
		{
			if (isPaused)
			{
				if (keyPressed->scancode == Keyboard::Scan::P)
				{
					isPaused = false;
					inputs.pausePressed = false;
				}
				else if (keyPressed->scancode == Keyboard::Scan::Escape)
				{
					isRunning = false;
					transitionToScene = Scene::Scenes::Exit;
				}
				continue;
			}

			inputs.reset();

			switch (keyPressed->scancode)
			{
			case Keyboard::Scan::Enter:
				inputs.enterPressed = true;
				if (levelWon)
				{
					if (game != nullptr && game->getCurrentWave() >= game->getMaxWaves())
					{
						if (game != nullptr) game->setVictory(true);
						isRunning = false;
						transitionToScene = Scene::Scenes::End;
					}
					else
					{
						isRunning = false;
						transitionToScene = Scene::Scenes::Transition;
					}
				}
				else if (gameOver)
				{
					isRunning = false;
					transitionToScene = Scene::Scenes::End;
				}
				break;

			case Keyboard::Scan::Z:

				inputs.archerTowerSelected = true;
				break;

			case Keyboard::Scan::X:

				inputs.mageTowerSelected = true;
				break;

			case Keyboard::Scan::C:

				inputs.plagueSelected = true;
				break;

			case Keyboard::Scan::V:

				inputs.sacredLightSelected = true;
				break;

			case Keyboard::Scan::Escape:
				isRunning = false;
				transitionToScene = Scene::Scenes::Exit;
				break;

			case Keyboard::Scan::P:
				isPaused = !isPaused;
				inputs.pausePressed = isPaused;
				break;

			default:
				break;
			}
		}

		inputs.leftMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);
	}
}

void GameScene::update()
{
	static constexpr float MANA_REGEN_INTERVAL = 0.2f;
	static constexpr float MIN_SPAWN_INTERVAL = 1.0f;
	static constexpr float MAX_SPAWN_INTERVAL = 3.0f;

	if (inputs.pausePressed)
	{
		hud.setSpecialStateText("Pause");
		hud.update(manaAmount, (
			game != nullptr) ? game->getScore() : 0, demonsKilled, currentWaveNumber,
			(game != nullptr) ? game->getScore() : 0,
			inputs.archerTowerSelected, inputs.mageTowerSelected,
			inputs.sacredLightSelected, inputs.plagueSelected, isPaused);
			return;
	}
	else if (!levelWon)
	{
		hud.setSpecialStateText("");
	}

	if (!levelWon && !gameOver)
	{
		manaTimer += deltaTime;

		if (manaTimer >= MANA_REGEN_INTERVAL)
		{
			manaAmount++;
			manaTimer = 0.0f;
		}
	}

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
		if (kingTower->isDead())
		{
			gameOver = true;
			if (game != nullptr)
			{
				game->setVictory(false);
				game->setGameOver(true);
			}
			return;
		}
	}

	for (int i = 0; i < NUM_TOWERS * NUM_TOWERS_TYPE; i++)
	{
		if (towers[i] != nullptr && towers[i]->isActive())
		{
			towers[i]->update(deltaTime);
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
			demons[freeSlot]->spawn(getDemonSpawnPosition(), waypoints[0], currentWaveNumber);
			spawnTimer = 0.0f;
			nextSpawnTime = MIN_SPAWN_INTERVAL + static_cast<float>(rand()) / RAND_MAX * (MAX_SPAWN_INTERVAL - MIN_SPAWN_INTERVAL); // Aide de ChatGPT pour faire un random entre 2 float
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

	updateProjectiles();

	if (levelWon)
	{
		hud.setSpecialStateText("- Vague Terminée - \n - Appuyez sur Enter -");
	}
	else
	{
		hud.setSpecialStateText("");
	}
	hud.update(manaAmount, (game != nullptr) ? game->getScore() : 0, demonsKilled, currentWaveNumber, (game != nullptr) ? game->getScore() : 0,
		inputs.archerTowerSelected, inputs.mageTowerSelected,
		inputs.sacredLightSelected, inputs.plagueSelected, isPaused);
}

void GameScene::draw()
{
	// Toujours important d'effacer l'écran précédent
	renderWindow.clear();
	renderWindow.draw(*map);

	if (kingTower != nullptr && kingTower->isActive())
	{
		kingTower->draw(renderWindow);
	}

	drawDemons();

	for (int i = 0; i < towerEmplacementCount; i++)
	{
		if (towersEmplacement[i] != nullptr && towersEmplacement[i]->isActive())
		{
			towersEmplacement[i]->draw(renderWindow);
		}
	}

	for (int i = 0; i < totalTowersCount; i++)
	{
		if (towers[i] != nullptr && towers[i]->isActive())
		{
			towers[i]->draw(renderWindow);
		}
	}
	
	drawProjectiles();
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

	for (int i = 0; i < waypointCount; i++)
	{
		delete waypoints[i];
	}

	if (kingTower != nullptr)
	{
		delete kingTower;
		kingTower = nullptr;
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

	for (int i = 0; i < towerEmplacementCount; i++)
	{
		if (towersEmplacement[i] != nullptr)
			delete towersEmplacement[i];
	}

	return true;
}

void GameScene::notify(Subject* subject, EventType eventType)
{
	static constexpr int MANA_KILL_REWARD = 25;
	if (eventType == EventType::DemonKilled)
	{
		if (levelWon || gameOver)
		{
			return;
		}
		demonsKilled++;

		manaAmount += MANA_KILL_REWARD;

		if (demonsKilled >= DEMON_TO_SPAWN)
		{
			levelWon = true;
		}
	}
	else if (eventType == EventType::TowerActivated)
	{

		Tower* tower = dynamic_cast<Tower*>(subject);
		if (tower != nullptr)
		{
			int cost = tower->getPrice();
	
			if (manaAmount >= cost)
			{
				manaAmount -= cost;
			}
		}
	}
	else if (eventType == EventType::TowerDeactivated)
	{
		if (subject == kingTower)
		{
			gameOver = true;
			if (game != nullptr)
			{
				game->setVictory(false);
				game->setGameOver(true);
			}
		}
	}
	else if (eventType == EventType::WaveFinished)
	{
		levelWon = true;
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
		for (int i = 0; i < waypointCount - 1; i++)
		{
			Waypoint* waypoint = waypoints[i];
			if (waypoint != nullptr && waypoint->getNextWaypoint() != nullptr)
			{
				CircleShape waypointShape(10.0f);
				waypointShape.setFillColor(Color::Red);
				waypointShape.setPosition(waypoint->getNextWaypoint()->getPosition());
				renderWindow.draw(waypointShape);
			}
		}
	}
}

void GameScene::handleSpells()
{
	static constexpr int MAX_TARGETS = 100;
	static constexpr int SACRED_LIGHT_MANA_COST = 60;
	static constexpr int PLAGUE_MANA_COST = 20;
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
			if (targetCount >= MAX_TARGETS) break;
			targets[targetCount] = demon;
			targetCount++;
		}
	}

	for (int i = 0; i < totalTowersCount; i++)
	{
		Tower* tower = towers[i];
		if (tower != nullptr && tower->isActive())
		{
			if (targetCount >= MAX_TARGETS) break;
			targets[targetCount] = tower;
			targetCount++;
		}
	}

	// Sacred Light
	if (inputs.sacredLightSelected && !sacredLight.isActive() && manaAmount >= SACRED_LIGHT_MANA_COST)
	{
		sacredLight.cast(
			mouseWorldPos,
			targets,
			targetCount
		);
		manaAmount -= SACRED_LIGHT_MANA_COST;
	}

	// Plague
	if (inputs.plagueSelected && !plague.isActive() && manaAmount >= PLAGUE_MANA_COST)
	{
		plague.cast(
			mouseWorldPos,
			targets,
			targetCount
		);
		manaAmount -= PLAGUE_MANA_COST;
	}

	inputs.leftMousePressed = false;
}

void GameScene::handleBuilding()
{
	static constexpr float PLACEMENT_RADIUS = 50.f;
	if (!inputs.leftMousePressed) return;
	if (!inputs.archerTowerSelected && !inputs.mageTowerSelected) return;

	Vector2f mouseWorldPos = renderWindow.mapPixelToCoords(Mouse::getPosition(renderWindow));

	//Recherche de l'emplacement cliqué
	TowerEmplacement* selectedEmplacement = nullptr;
	for (int i = 0; i < towerEmplacementCount; i++)
	{
		TowerEmplacement* towerEmplacement = towersEmplacement[i];
		if (towerEmplacement != nullptr && !towerEmplacement->isOccupied())
		{
			float dist = (mouseWorldPos - towerEmplacement->getPosition()).length();
			if (dist < PLACEMENT_RADIUS)
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
	for (int i = 0; i < totalTowersCount; i++)
	{
		Tower* tower = towers[i];
		if (tower != nullptr && !tower->isActive() && tower->getType() == desiredType)
		{
			newTower = tower;
			break;
		}
	}

	if (newTower == nullptr) return;
	if (manaAmount < newTower->getPrice()) return;

	newTower->setPosition(selectedEmplacement->getPosition());
	newTower->activate();
	selectedEmplacement->placeTower(newTower);
}