#include "Tower.h"
#include "ContentPipeline.h" 
#include "KingTower.h"
#include "Spell.h"

Tower::Tower()
{
}

Tower::~Tower()
{
}

Tower* Tower::createKingTower()
{
	return new KingTower();
}

TowersType Tower::getType() const
{
	return type;
}

void Tower::setType(const TowersType type)
{
	this->type = type;
}

int Tower::getLifePoints() const
{
	return getHealth();
}

void Tower::setLifePoints(const int desiredLifePoints)
{
	if (getMaxHealth() <= 0)
	{
		initDamageable(desiredLifePoints);
		return;
	}

	Damageable::setCurrentHealth(desiredLifePoints);
}

void Tower::heal(const int amount)
{
	Damageable::heal(amount);
}

void Tower::takeDamage(const int amount)
{
	Damageable::takeDamage(amount);
}

bool Tower::isDead() const
{
	return Damageable::isDead();
}

void Tower::onHealthChanged()
{
	GameObject::setHealth(getHealth(), getMaxHealth());
}

void Tower::onDeath()
{
	notifyAllObservers(EventType::TowerDeactivated);
	deactivate();

}

void Tower::notify(Subject* subject, EventType eventType)
{

}