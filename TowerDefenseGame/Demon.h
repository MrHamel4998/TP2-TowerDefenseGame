#pragma once
#include <SFML/Graphics.hpp>
#include "Waypoint.h"
#include "AnimatedObject.h"
using namespace sf;
/*
Metrics du Demon (� effacer � la fin)
- Rectangle d'animation: 100 X 50
- Rayon du cercle de collision: hauteur du rectangle d'animation (50) divis� par 2
- Vitesse d'animation: 1 frame par 0.1 /secs
- Vitesse de d�placement: 0.9 + 0.1 X Num�ro de vague (Si utilisation du DeltaTime, faire x60)
- Points de vie: 60 (50 pour la version � un d�veloppeur);
- Cadence de tir: un tir par (1.05 - 0.05 * num�ro de vague) seconde.
- Port�e du tir: 250
- Position de d�part des d�mons au niveau 1: 610, -100
- Position de d�part des d�mons au niveau 2: -100, 410
*/


class Demon : public AnimatedObject
{
public:
	   static constexpr int RECTANGLE_SIZE_X = 100;
	   static constexpr int RECTANGLE_SIZE_Y = 50;

	   enum AnimationIndex { FLY = 0, DEATH = 1, ANIMATION_COUNT = 2 };

	   Demon();
	   void spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber);
	   void update(float deltaTime);
	   bool init();
	   void takeDamage(int damage);

private:
	   static const int BASE_HEALTH = 60;
	   int health;
	   int maxHp;

	   float speed;

	   Waypoint* currentTargetWaypoint;
	   AnimationIndex currentAnimation;

	bool isDying = false;
};

