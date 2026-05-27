#pragma once
#include <SFML/Audio.hpp>
#include "Scene.h"
#include "Hud.h"
#include "Inputs.h"
#include "Waypoint.h"
#include "Demon.h"
#include "IObserver.h"
#include "SacredLight.h"
#include "Plague.h"
#include "TowerEmplacement.h"
#include "Tower.h"
#include "Projectile.h"
#include "ShootingTower.h"

class Game;

class GameScene : public Scene, public IObserver
{
public:

	GameScene(RenderWindow& renderWindow, Game* game = nullptr);
	Scenes run() override;
	bool init() override;
	void notify(Subject* subject, EventType eventType) override;

protected:
	virtual Maps getMapId() const = 0;

	virtual Vector2f getDemonSpawnPosition() const = 0;
	virtual Vector2f getKingTowerPosition() const = 0;
	virtual const Vector2f* getWaypointPositions() const = 0;
	virtual const Vector2f* getTowerEmplacementPositions() const = 0;

	virtual int getWaypointPositionsCount() const = 0;
	virtual int getTowerEmplacementPositionsCount() const = 0;

	virtual void configureWaypoints() = 0;

protected:
	static const int MAX_WAYPOINTS = 14;
	static const int NUM_TOWERS_TYPE = 2;
	static const int MAX_TOWERS_EMPLACEMENT = 9;
	static const int NUM_TOWERS = MAX_TOWERS_EMPLACEMENT;
	static const int NUM_DEMONS_TOTAL = 20;
	static const int DEMON_TO_SPAWN = 50;

	void getInputs() override;
	void update() override;
	void draw() override;
	bool unload() override;

	void handleSpells();
	void drawWaypoints();
	void handleBuilding();
	void updateProjectiles();
	void drawDemons();
	void drawProjectiles();

	View view;
	Hud hud;
	Game* game;
	Inputs inputs;

	Sprite* map = nullptr;
	SacredLight sacredLight;
	Plague plague;

	Waypoint* waypoints[MAX_WAYPOINTS];
	int waypointCount = 0;

	Demon* demons[NUM_DEMONS_TOTAL];

	Tower* kingTower;
	ShootingTower* towers[NUM_TOWERS * NUM_TOWERS_TYPE];
	TowerEmplacement* towersEmplacement[MAX_TOWERS_EMPLACEMENT];

	int towerEmplacementCount = 0;
	int totalTowersCount = 0;

	GameObject* targets[NUM_DEMONS_TOTAL + MAX_TOWERS_EMPLACEMENT];
	int targetCount = 0;

	static const int NUM_PROJECTILES = 300;
	Projectile* projectiles[NUM_PROJECTILES];

	Music music;
	const String MUSIC_PATH_ARRAY[NBR_MUSIC] = {
		"Ressources\\Sounds\\Music\\Theme01.ogg",
		"Ressources\\Sounds\\Music\\Theme02.ogg",
		"Ressources\\Sounds\\Music\\Theme03.ogg"
	};

	float manaTimer = 0;
	int manaAmount = 500;
	int currentWaveNumber = 1;
	float spawnTimer = 0.0f;
	float nextSpawnTime = 0.0f;
	int demonsSpawned = 0;
	int demonsKilled = 0;

	bool levelWon = false;
	bool gameOver = false;
	bool isPaused = false;
};