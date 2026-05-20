#include "Projectile.h"
#include "ContentPipeline.h"
#include <iostream>

Projectile::Projectile()
{
	deactivate();
}

void Projectile::launch(ProjectileType type, const Vector2f& startPosition, GameObject* target, int waveNumber)
{
	this->target = target;
	this->type = type;
	reachedTarget = false;

	configureStats(type, waveNumber);
	setPosition(startPosition);
	activate();
}

void Projectile::update(float deltaTime)
{
	if (!isActive() || target == nullptr || !target->isActive())
	{
		deactivate();
		return;
	}

	Vector2f direction = target->getPosition() - getPosition();
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance <= 1 || isCircleColliding(*target))
	{
		reachedTarget = true;
		deactivate();
		return;
	}

	if (distance > 0.0f)
	{
		direction /= distance;
		move(direction * speed * deltaTime);
		setRotation(degrees(atan2(direction.y, direction.x) * 180.0f / 3.14159265f));
	}
}

GameObject* Projectile::getTarget() const
{
	return target;
}

int Projectile::getDamage() const
{
	return damage;
}

bool Projectile::hasReachedTarget() const
{
	return reachedTarget;
}

bool Projectile::consumeImpact()
{
	if (!reachedTarget)
	{	
		return false;
	}

	reachedTarget = false;
	return true;
}

void Projectile::configureStats(ProjectileType type, int waveNumber)
{
	const ContentPipeline& content = ContentPipeline::getInstance();

	switch (type)
	{
	case Arrow:
		setTexture(content.getArrowTexture());
		speed = 15.0f * 60.0f;
		damage = getRandomDamage(1, 4);
		break;
	case Blast:
		setTexture(content.getBlastTexture());
		speed = 8.0f * 60.0f;
		damage = getRandomDamage(1, 12);
		break;
	case Fireball:
		setTexture(content.getFireballTexture());
		speed = 10.0f * 60.0f;
		damage = getRandomDamage(1, 10);
		break;
	default:
		setTexture(content.getArrowTexture());
		speed = 15.0f * 60.0f;
		damage = getRandomDamage(1, 4);
		break;
	}

	setCollisionCircleRadius(getTexture().getSize().x / 2.0f);
}

int Projectile::getRandomDamage(int minimumDamage, int maximumDamage) const
{
	return minimumDamage + rand() % (maximumDamage - minimumDamage + 1);
}

