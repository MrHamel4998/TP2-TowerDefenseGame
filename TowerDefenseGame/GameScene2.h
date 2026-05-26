#pragma once
#include "Scene.h"
#include "Hud.h"
#include "Inputs.h"
#include "Waypoint.h"
#include "Demon.h"
#include "IObserver.h"
#include "SacredLight.h"
#include "Plague.h"
#include "Tower.h"
#include "Projectile.h"
#include "TowerEmplacement.h"
#include "ShootingTower.h"

class Game;

/*
Metrics du level 2 (à effacer à la fin)
- Position de la tour du roi: 1138, 564
- Position des emplacements de tour: (110, 620), (228, 320), (444, 780), (362, 530), (610, 222), (998, 270), (630, 460), (935, 520), (798, 760)
- Position des waypoints: (88.f, 412.f), (168.f, 465.f), (222.f, 588.f), (308.f, 670.f), (424.f, 668.f), (double sortie: 510.f, 590.f);
                          (478.f, 468.f), (516.f, 380.f), (594.f, 360.f), (806.f, 368.f), (1140.f, 450.f), (660.f, 598.f), (804.f, 650.f), (1140.f, 680.f)
- Position de départ des démons: -100, 410

- 50 éliminations de démons (donc 50 spawns pour gagner la scène)
*/

class GameScene2 : public Scene, public IObserver
{
public:

    GameScene2(RenderWindow& renderWindow, Game* game = nullptr);
    Scenes run() override;
    bool init() override;
    void notify(Subject* subject, EventType eventType) override;

private:
    static const int NUM_WAYPOINTS = 14;
    static const int NUM_TOWERS_EMPLACEMENT = 9;
    static const int NUM_TOWERS_TYPE = 2;
    static const int NUM_TOWERS = NUM_TOWERS_EMPLACEMENT;
    static const int NUM_DEMONS_TOTAL = 20;
    static const int DEMON_TO_SPAWN = 5;
    static constexpr Vector2f DEMON_SPAWN_POSITION = Vector2f(-100, 410);

    void getInputs() override;
    void update() override;
    void draw() override;
    bool unload() override;

    void handleBuilding();
    void handleSpells();
    void drawWaypoints();

    View view;
	Game* game;
    Hud hud;
    Inputs inputs;

    Sprite* map = nullptr;
    SacredLight sacredLight;
    Plague plague;

    Waypoint* waypoints[NUM_WAYPOINTS];
    Demon* demons[NUM_DEMONS_TOTAL];
    Tower* kingTower;
    ShootingTower* towers[NUM_TOWERS * NUM_TOWERS_TYPE];
    TowerEmplacement* towersEmplacement[NUM_TOWERS_EMPLACEMENT];

    GameObject* targets[NUM_DEMONS_TOTAL + NUM_TOWERS_EMPLACEMENT];
    int targetCount = 0;

    static const int NUM_PROJECTILES = 50;
    Projectile* projectiles[NUM_PROJECTILES];

    int scorePoints = 0;
    int currentWaveNumber = 2;
    float spawnTimer = 0.0f;
    float nextSpawnTime = 0.0f;
    int demonsSpawned = 0;
    int demonsKilled = 0;

    bool levelWon = false;
    bool gameOver = false;
    bool isPaused = false;
};
