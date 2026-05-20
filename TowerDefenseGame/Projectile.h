#pragma once
#include "Constants.h"
#include "GameObject.h"

/*
Metrics du Projectile (à effacer à la fin)

- Vitesse de déplacement (framerate 60 fps): Arrow: 15, blast 8, fireball: 10
(Si utilisation du DeltaTime, faire x60 à ces valeurs)

- Dommages: les projectiles font les dommages random suivants
  Arrow: 1 à 4, Blast 1 à 12, Fireball: 1 à 10

- Cadence de tir: un tir à tous les 
  Arrow: 48 rafraichissements / 0.8 secondes
  Blast: 90 rafraichissements / 1.5 secondes
  Fireball: 63 - 3 * wave / 1.05 secondes - 0.05 * wave
*/

class Projectile : public GameObject
{
public:
	Projectile();

	void launch(ProjectileType type, const Vector2f& startPosition, GameObject* target, int waveNumber = 1);
	void update(float deltaTime);

	GameObject* getTarget() const;
	int getDamage() const;
	bool hasReachedTarget() const;
	bool consumeImpact();

	void configureStats(ProjectileType type, int waveNumber);
	int getRandomDamage(int minimumDamage, int maximumDamage) const;

private:
	Vector2f position;
	Vector2f velocity;

	float speed = 0.0f;
	int damage = 0;

	GameObject* target = nullptr;
	ProjectileType type = Arrow;

	bool reachedTarget = false;
};

