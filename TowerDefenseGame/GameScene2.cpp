#include "GameScene2.h"
#include "ContentPipeline.h"
#include "Game.h"
#include "Constants.h"
#include <iostream>

GameScene2::GameScene2(RenderWindow& renderWindow, class Game* game) 
    : Scene(renderWindow), game(game)
{
    view = renderWindow.getDefaultView();
}

Scene::Scenes GameScene2::run()
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

bool GameScene2::init()
{
    inputs.reset();

    map = new Sprite(ContentPipeline::getInstance().getMapTexture(Maps::Map2));

    hud.hudInit(ContentPipeline::getInstance().getHudmaskTexture(), ContentPipeline::getInstance().getComiciFont());

    waypoints[0] = new Waypoint(Vector2f(88.f, 412.f));
    waypoints[1] = new Waypoint(Vector2f(168.f, 465.f));
    waypoints[2] = new Waypoint(Vector2f(222.f, 588.f));
    waypoints[3] = new Waypoint(Vector2f(308.f, 670.f));
    waypoints[4] = new Waypoint(Vector2f(424.f, 668.f));
    waypoints[5] = new Waypoint(Vector2f(510.f, 590.f));

    // Chemin du haut
    waypoints[6] = new Waypoint(Vector2f(478.f, 468.f));
    waypoints[7] = new Waypoint(Vector2f(516.f, 380.f));
    waypoints[8] = new Waypoint(Vector2f(594.f, 360.f));
    waypoints[9] = new Waypoint(Vector2f(806.f, 368.f));
    waypoints[10] = new Waypoint(Vector2f(1140.f, 450.f));

    // Chemin de droite
    waypoints[11] = new Waypoint(Vector2f(660.f, 598.f));
    waypoints[12] = new Waypoint(Vector2f(804.f, 650.f));
    waypoints[13] = new Waypoint(Vector2f(1140.f, 680.f));

    for (int i = 0; i < 5; i++)
    {
        waypoints[i]->setNextWaypoint(waypoints[i + 1]);
    }

    waypoints[5]->setNextWaypoint(waypoints[6]);      // Chemin principal (haut)
    waypoints[5]->setAlternativeWaypoint(waypoints[11]); // Chemin alternatif (droite)

    // Chemin du haut
    for (int i = 6; i < 10; i++)
    {
        waypoints[i]->setNextWaypoint(waypoints[i + 1]);
    }

    // Chemin de droite
    for (int i = 11; i < 13; i++)
    {
        waypoints[i]->setNextWaypoint(waypoints[i + 1]);
    }

    for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
    {
        demons[i] = new Demon();
    }

    for (int i = 0; i < NUM_PROJECTILES; i++) 
    {
        projectiles[i] = nullptr;
    }
    
    Vector2f emplacementPositions[NUM_TOWERS_EMPLACEMENT] = {
        Vector2f(110, 620), Vector2f(228, 320), Vector2f(444, 780), Vector2f(362, 530), 
        Vector2f(610, 222), Vector2f(998, 270), Vector2f(630, 460), Vector2f(935, 520), 
        Vector2f(798, 760)
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
    kingTower->setPosition(Vector2f(1138, 564));
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
    levelWon = false;
    gameOver = false;
    currentWaveNumber = (game != nullptr) ? game->getCurrentWave() : 1;

    return true;
}

void GameScene2::getInputs()
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

        if (const Event::KeyPressed* keyPressed =
            event->getIf<Event::KeyPressed>())
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

            inputs.archerTowerSelected = false;
            inputs.mageTowerSelected = false;
            inputs.sacredLightSelected = false;
            inputs.plagueSelected = false;

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

            case Keyboard::Scan::P:
                isPaused = !isPaused;
                inputs.pausePressed = isPaused;
                break;
            }
        }

        inputs.leftMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);
    }
}

void GameScene2::update()
{
    if (inputs.pausePressed)
    {
        return;
    }

    manaTimer += deltaTime;

    if (manaTimer >= 0.2f)
    {
        manaAmount++;
        manaTimer = 0.0f;
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
            towers[i]->shoot(deltaTime, demons, NUM_DEMONS_TOTAL, projectiles, NUM_PROJECTILES, currentWaveNumber);
        }
    }

    spawnTimer += deltaTime;

    if (spawnTimer >= nextSpawnTime && demonsSpawned < DEMON_TO_SPAWN) 
    {
        int freeSlot = -1;
        for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
        {
            if (demons[i] == nullptr || !demons[i]->isActive())
            {
                freeSlot = i;
                break;
            }
        }

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
                else if (Demon* targetDemon = dynamic_cast<Demon*>(projectiles[i]->getTarget()))
                {
                    if (targetDemon->isActive())
                    {
                        targetDemon->takeDamage(projectiles[i]->getDamage());
                        scorePoints += projectiles[i]->getDamage();
                    }
                }

                projectiles[i]->consumeImpact();
            }
        }
    }

    if (scorePoints > highScore)
    {
        highScore = scorePoints;
    }

    if (levelWon)
    {
        hud.setSpecialStateText("Appuyez sur Enter pour continuer");
    }
    if (isPaused)
    {
        hud.setSpecialStateText("Pause");
    }

    hud.update(manaAmount, scorePoints, demonsKilled, currentWaveNumber, highScore,
        inputs.archerTowerSelected, inputs.mageTowerSelected,
        inputs.sacredLightSelected, inputs.plagueSelected, isPaused);
}

void GameScene2::draw()
{
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

    for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
    {
        if (demons[i] != nullptr && demons[i]->isActive())
        {
            demons[i]->draw(renderWindow);
        }
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

bool GameScene2::unload()
{
    Subject::removeAllObservers();

    if (map != nullptr) delete map;

    for (int i = 0; i < NUM_WAYPOINTS; i++)
    {
        if (waypoints[i] != nullptr)
        {
            delete waypoints[i];
        }
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

    for (int i = 0; i < NUM_PROJECTILES; i++)
    {
        if (projectiles[i] != nullptr)
        {
            delete projectiles[i];
        }
    }

    return true;
}

void GameScene2::notify(Subject* subject, EventType eventType)
{
    if (eventType == EventType::DemonKilled)
    {
        demonsKilled++;

        manaAmount += 25;

        if (demonsKilled >= DEMON_TO_SPAWN)
        {
            levelWon = true;
        }
    }
    else if (eventType == EventType::DemonDamageTaken)
    {
        // TODO
    }
    else if (eventType == EventType::TowerActivated)
    {
        // TODO
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

void GameScene2::drawWaypoints()
{
    if (!inputs.showWaypoints)
    {
        return;
    }
    if (inputs.showWaypoints)
    {
        for (int i = 0; i < NUM_WAYPOINTS - 1; i++)
        {
            if (waypoints[i] != nullptr && waypoints[i]->getNextWaypoint() != nullptr)
            {
                CircleShape waypointShape(10.0f);
                waypointShape.setFillColor(Color::Red);
                waypointShape.setPosition(waypoints[i]->getNextWaypoint()->getPosition());
                renderWindow.draw(waypointShape);
            }
        }
    }
}

void GameScene2::handleSpells()
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
    if (inputs.sacredLightSelected && !sacredLight.isActive() && manaAmount >= 60)
    {
        sacredLight.cast(
            mouseWorldPos,
            targets,
            targetCount
        );
        manaAmount -= 60;
    }

    // Plague
    if (inputs.plagueSelected && !plague.isActive() && manaAmount >= 20)
    {
        plague.cast(
            mouseWorldPos,
            targets,
            targetCount
        );
        manaAmount -= 20;
    }

    inputs.leftMousePressed = false;
}

void GameScene2::handleBuilding()
{
    if (!inputs.leftMousePressed) return;
    if (!inputs.archerTowerSelected && !inputs.mageTowerSelected) return;

    if (inputs.archerTowerSelected && manaAmount < 70)
    {
        return;
    }
    else if (inputs.archerTowerSelected && manaAmount >= 70)
    {
        manaAmount -= 70;
    }
    if (inputs.mageTowerSelected && manaAmount < 100)
    {
        return;
    }
    else if (inputs.mageTowerSelected && manaAmount >= 100)
    {
        manaAmount -= 100;
    }

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