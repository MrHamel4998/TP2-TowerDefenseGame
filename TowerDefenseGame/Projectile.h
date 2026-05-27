#pragma once
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile();
	~Projectile();

	void launch(ProjectileType type, const Vector2f& startPosition, GameObject* target, int waveNumber = 1);
	void update(float deltaTime);
	void setHitSound(const SoundBuffer& buffer);

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

	Sound* hitSound = nullptr;
	GameObject* target = nullptr;
	ProjectileType type = Arrow;

	bool reachedTarget = false;
};