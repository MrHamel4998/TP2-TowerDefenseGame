#pragma once
#include <SFML/Graphics.hpp>
#include "Waypoint.h"
#include "AnimatedObject.h"
#include "Subject.h"
#include "Damageable.h"
#include "IObserver.h"
using namespace sf;
/*
Metrics du Demon (à effacer à la fin)
- Rectangle d'animation: 100 X 50
- Rayon du cercle de collision: hauteur du rectangle d'animation (50) divisée par 2
- Vitesse d'animation: 1 frame par 0.1 /secs
- Vitesse de déplacement: 0.9 + 0.1 X Numéro de vague (Si utilisation du DeltaTime, faire x60)
- Points de vie: 60 (50 pour la version à un développeur);
- Cadence de tir: un tir par (1.05 - 0.05 * numéro de vague) seconde.
- Portée du tir: 250
- Position de départ des démons au niveau 1: 610, -100
- Position de départ des démons au niveau 2: -100, 410
*/


class Demon : public AnimatedObject, public Subject, public Damageable, public IObserver
{
public:
    static constexpr int RECTANGLE_SIZE_X = 100;
    static constexpr int RECTANGLE_SIZE_Y = 50;

    enum AnimationIndex { FLY = 0, DEATH = 1, ANIMATION_COUNT = 2 };

    Demon();
    void spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber);
    void update(float deltaTime);
    bool init();

    void notify(Subject* subject, EventType eventType) override;
private:
    static const int BASE_HEALTH = 60;

    float speed;
	float sacredLightRatio = 1.0f;
	float sacredLightTimer = 0.0f;
	float plagueDamageMultiplier = 1.0f;
    float plagueTimer = 0.0f;

    Waypoint* currentTargetWaypoint;

    bool isDying = false;

    void onHealthChanged() override;
    void onDeath() override;
};
