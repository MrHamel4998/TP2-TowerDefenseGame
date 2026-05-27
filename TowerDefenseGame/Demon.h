#pragma once
#include <SFML/Graphics.hpp>
#include "Waypoint.h"
#include "AnimatedObject.h"
#include "Subject.h"
#include "Damageable.h"
#include "IObserver.h"
#include "ShootingTower.h"
using namespace sf;
class Tower;
class Projectile;

class Demon : public AnimatedObject, public Subject, public Damageable, public IObserver
{
public:
	static constexpr int RECTANGLE_SIZE_X = 100;
	static constexpr int RECTANGLE_SIZE_Y = 50;

	enum AnimationIndex { FLY = 0, DEATH = 1, ANIMATION_COUNT = 2 };

	Demon();
	~Demon();
	void spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber);
	void update(float deltaTime);
	bool init();
	void getAttacksound();

	void notify(Subject* subject, EventType eventType) override;
	void shoot(float deltaTime, ShootingTower* towers[], Tower* kingTower, int towerCount, Projectile* projectiles[], int projectileCount, int waveNumber);
private:
	static const int BASE_HEALTH = 60;

	float speed;
	float sacredLightRatio = 1.0f;
	float sacredLightTimer = 0.0f;
	float plagueDamageMultiplier = 1.0f;
	float plagueTimer = 0.0f;

	Waypoint* currentTargetWaypoint;

	bool isDying = false;
	bool reachedKingTower = false;

	void onHealthChanged() override;
	void onDeath() override;

	void updateTimers(float deltaTime);
	void handleMovement(float deltaTime);
	void handleWaypointArrival();
	void updateFlip();
	Sound* attackSound = nullptr;
	Sound* deathSound = nullptr;

	static constexpr float BASE_FIRE_RATE = 1.05f;
	static constexpr float FIRE_RATE_REDUCTION_PER_WAVE = 0.05f;
	static constexpr float BASE_FIRE_RANGE = 250.0f;

	float fireRate = BASE_FIRE_RATE;
	float fireRange = BASE_FIRE_RANGE;
	float fireTimer = 0.0f;
	ProjectileType projectileType = ProjectileType::Fireball;
};