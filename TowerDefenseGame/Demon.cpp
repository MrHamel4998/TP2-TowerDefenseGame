#include "Demon.h"
#include "ContentPipeline.h"
#include "Tower.h"
#include "Spell.h"
#include "SacredLight.h"
#include "Plague.h"
#include <iostream>

Demon::Demon()
{
	setTexture(ContentPipeline::getInstance().getDemonTexture());
	init();
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
	plagueTimer = 0.0f;
	initDamageable(BASE_HEALTH);
	setHealth(getHealth(), getMaxHealth());
	activate();
	Subject::addObserver(this);
}

void Demon::takeDamage(int damage)
{
	if (damage <= 0 || isDead())
	{
		return;
	}

	damage = static_cast<int>(damage * plagueDamageMultiplier);

	Damageable::takeDamage(damage);
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
	if (currentTargetWaypoint == nullptr)
	{
		takeDamage(getHealth()); // Mettre la santé à 0 pour déclencher l'animation de mort
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

	if (currentTargetWaypoint->hasAlternative())
	{
		if (rand() % 2 == 0)
		{
			currentTargetWaypoint = currentTargetWaypoint->getNextWaypoint();
		}
		else
		{
			currentTargetWaypoint = currentTargetWaypoint->getAlternativeWaypoint();
		}
	}
	else
	{
		currentTargetWaypoint = currentTargetWaypoint->getNextWaypoint();
	}
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

void Demon::onHealthChanged()
{
	setHealth(getHealth(), getMaxHealth());
	Subject::notifyAllObservers(EventType::DemonDamageTaken);
}

void Demon::onDeath()
{
	Subject::notifyAllObservers(EventType::DemonKilled);
}