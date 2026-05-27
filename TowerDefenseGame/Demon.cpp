#include "Demon.h"
#include "ContentPipeline.h"
#include "Projectile.h"
#include "Spell.h"
#include "SacredLight.h"
#include "Plague.h"
#include <iostream>
#include "ShootingTower.h"

Demon::Demon()
{
	setTexture(ContentPipeline::getInstance().getDemonTexture());
	init();
}

Demon::~Demon()
{
	if (attackSound != nullptr) delete attackSound;
}

void Demon::getAttacksound()
{
	attackSound->play();
}

bool Demon::init()
{
    if (!initAnimationsStructure(ANIMATION_COUNT, Vector2i(RECTANGLE_SIZE_X, RECTANGLE_SIZE_Y))) return false;
	setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	if (!initAnimation(FLY, 5, 0.1f, AnimationType::Circular)) return false;
	if (!initAnimation(DEATH, 5, 0.1f, AnimationType::Linear)) return false;
	setActiveAnimation(FLY, true);
	   
	getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());

	attackSound = new Sound(ContentPipeline::getInstance().getDemonAttackSoundBuffer());
	   
	return true;
}

void Demon::spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber)
{
	setTexture(ContentPipeline::getInstance().getDemonTexture());
	setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	setActiveAnimation(FLY, true);
	setPosition(position);
	this->currentTargetWaypoint = firstWaypoint;
	this->speed = (0.9f + 0.1f * waveNumber) * 60.0f;
	isDying = false;
	reachedKingTower = false;
	plagueTimer = 0.0f;
	initDamageable(BASE_HEALTH);
	setHealth(getHealth(), getMaxHealth());

	fireRate = BASE_FIRE_RATE - FIRE_RATE_REDUCTION_PER_WAVE * waveNumber;
	if (fireRate < 0.1f) fireRate = 0.1f;
	fireRange = BASE_FIRE_RANGE;
	fireTimer = 0.0f;

	activate();
	Subject::addObserver(this);
}

void Demon::update(float deltaTime)
{
	if (isDying)
	{
		updateAnimation(deltaTime);
		if (isCurrentLinearAnimationIsOver())
		{
			deactivate();
		}
		return;
	}

	if (isDead())
	{
		setActiveAnimation(DEATH, true);
		isDying = true;
		updateAnimation(deltaTime);
		return;
	}

	updateAnimation(deltaTime);
	updateTimers(deltaTime);
	handleMovement(deltaTime);
	updateFlip();
}

void Demon::updateTimers(float deltaTime)
{
	if (sacredLightTimer > 0.0f) {
		sacredLightTimer -= deltaTime;
		if (sacredLightTimer <= 0.0f) {
			sacredLightRatio = 1.0f;
			setColor(Color::White);
		}
	}

	if (plagueTimer > 0.0f) {
		plagueTimer -= deltaTime;
		if (plagueTimer <= 0.0f) {
			plagueDamageMultiplier = 1.0f;
			setColor(Color::White);
		}
	}
}

void Demon::handleMovement(float deltaTime)
{
	if (reachedKingTower || currentTargetWaypoint == nullptr)
	{
		return;
	}

	Vector2f currentPos = getPosition();
	Vector2f targetPos = currentTargetWaypoint->getPosition();
	Vector2f direction = targetPos - currentPos;
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 2.0f)
	{
		handleWaypointArrival();
		return;
	}

	if (distance > 0.0f)
	{
		direction /= distance;

		float movement = speed * deltaTime * sacredLightRatio;
		move(direction * movement);
	}
}

void Demon::handleWaypointArrival()
{
	if (currentTargetWaypoint == nullptr) return;

	Waypoint* nextWaypoint = nullptr;

	if (currentTargetWaypoint->hasAlternative())
	{
		nextWaypoint = (rand() % 2 == 0) ? currentTargetWaypoint->getNextWaypoint() : currentTargetWaypoint->getAlternativeWaypoint();
	}
	else
	{
		nextWaypoint = currentTargetWaypoint->getNextWaypoint();
	}

	if (nextWaypoint == nullptr)
	{
		reachedKingTower = true;
		currentTargetWaypoint = nullptr;
		return;
	}

	currentTargetWaypoint = nextWaypoint;
}

void Demon::updateFlip()
{
	if (currentTargetWaypoint == nullptr) return;

	Vector2f currentPos = getPosition();
	Vector2f targetPos = currentTargetWaypoint->getPosition();

	if (targetPos.x > currentPos.x)
	{
		setScale(Vector2f(1.0f, 1.0f));    // Waypoint à droite
	}
	else
	{
		setScale(Vector2f(-1.0f, 1.0f));   // Waypoint à gauche
	}
}

void Demon::notify(Subject* subject, EventType eventType)
{
	Spell* spell = dynamic_cast<Spell*>(subject);

	if (eventType != EventType::SpellCast || spell == nullptr || !spell->containsTarget(this))
	{
		return;
	}

	switch (spell->getSpellType())
	{
		case SpellType::SacredLightSpell:
		{
			SacredLight* sacredLight = dynamic_cast<SacredLight*>(spell);

			if (sacredLight == nullptr)
			{
				return;
			}

			takeDamage(sacredLight->getRandomDamage());
			setColor(sacredLight->getEffectColor());

			sacredLightRatio = 0.5f;
			sacredLightTimer = sacredLight->getLifetime();

			break;
		}

		case SpellType::PlagueSpell:
		{
			Plague* plague = dynamic_cast<Plague*>(spell);

			if (plague == nullptr)
			{
				return;
			}

			takeDamage(plague->getRandomDamage());
			setColor(plague->getEffectColor());

			plagueDamageMultiplier = 2.0f;
			plagueTimer = plague->getLifetime();

			break;
		}
	}
}

void Demon::shoot(float deltaTime, ShootingTower* towers[], Tower* kingTower, int towerCount, Projectile* projectiles[], int projectileCount, int waveNumber)
{
	if (isDying || isDead())
	{
		return;
	}

	fireTimer += deltaTime;
	if (fireTimer < fireRate)
	{
		return;
	}

	Tower* targetTower = nullptr;
	float closestDistanceSquared = fireRange * fireRange;
	Vector2f demonPosition = getPosition();

	for (int i = 0; i < towerCount + 1; i++)
	{
		Tower* tower;

		if (i == towerCount)
		{
			tower = kingTower;
		}
		else
		{
			tower = towers[i];
		}

		if (tower == nullptr || !tower->isActive())
		{
			continue;
		}

		Vector2f offset = tower->getPosition() - demonPosition;
		float distanceSquared = offset.x * offset.x + offset.y * offset.y;
		if (distanceSquared <= closestDistanceSquared)
		{
			closestDistanceSquared = distanceSquared;
			targetTower = tower;
		}
	}

	if (targetTower == nullptr)
	{
		return;
	}

	Projectile* projectile = nullptr;
	for (int i = 0; i < projectileCount; i++)
	{
		if (projectiles[i] == nullptr)
		{
			projectiles[i] = new Projectile();
			projectile = projectiles[i];
			break;
		}

		if (!projectiles[i]->isActive())
		{
			projectile = projectiles[i];
			break;
		}
	}

	if (projectile == nullptr)
	{
		return;
	}

	projectile->launch(projectileType, demonPosition, targetTower, waveNumber);
	fireTimer = 0.0f;
}

void Demon::onHealthChanged()
{
	setHealth(getHealth(), getMaxHealth());
	Subject::notifyAllObservers(EventType::DemonDamageTaken);
}

void Demon::onDeath()
{
	Subject::notifyAllObservers(EventType::DemonKilled);
}