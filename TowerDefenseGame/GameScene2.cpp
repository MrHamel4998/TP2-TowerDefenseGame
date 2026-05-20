#include "GameScene2.h"
#include "ContentPipeline.h"
#include <iostream>

GameScene2::GameScene2(RenderWindow& renderWindow) : Scene(renderWindow)
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

    for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
    {
        towers[i] = nullptr;
    }

    for (int i = 0; i < NUM_PROJECTILES; i++) 
    {
        projectiles[i] = nullptr;
    }

    Subject::addObserver(this);
    sacredLight.init();
    plague.init();

    isRunning = true;
    demonsKilled = 0;

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

        // Selection des sorts
        if (Keyboard::isKeyPressed(Keyboard::Key::V))
        {
            inputs.sacredLightSelected = true;
            inputs.plagueSelected = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::C))
        {
            inputs.plagueSelected = true;
            inputs.sacredLightSelected = false;
        }

        inputs.leftMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);
    }
}

void GameScene2::update()
{
    for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
    {
        if (demons[i] != nullptr && demons[i]->isActive())
        {
            demons[i]->update(deltaTime);
            demons[i]->shoot(deltaTime, towers, NUM_TOWERS_EMPLACEMENT, projectiles, NUM_PROJECTILES, currentWaveNumber);
        }
    }

    for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
    {
        if (towers[i] != nullptr && towers[i]->isActive())
        {
            towers[i]->update(deltaTime);
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

        sacredLight.update(deltaTime);
        plague.update(deltaTime);

        for (int i = 0; i < NUM_PROJECTILES; i++)
        {
            if (projectiles[i] != nullptr && projectiles[i]->isActive())
            {
                projectiles[i]->update(deltaTime);

                if (projectiles[i]->hasReachedTarget())
                {
                    projectiles[i]->consumeImpact();
                }
            }
        }
    }
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

    for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
    {
        if (towers[i] != nullptr && towers[i]->isActive())
        {
            towers[i]->draw(renderWindow);
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

    for (int i = 0; i < NUM_DEMONS_TOTAL; i++)
    {
        if (demons[i] != nullptr)
        {
            delete demons[i];
        }
    }

    for (int i = 0; i < NUM_TOWERS_EMPLACEMENT; i++)
    {
        if (towers[i] != nullptr)
        {
            delete towers[i];
        }
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

        if (demonsKilled >= DEMON_TO_SPAWN)
        {
            isRunning = false;
            transitionToScene = Scene::Scenes::End;
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
        // TODO
    }
    else if (eventType == EventType::WaveFinished)
    {
        // TODO
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