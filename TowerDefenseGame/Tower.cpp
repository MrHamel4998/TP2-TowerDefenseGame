#include "Tower.h"
#include "ContentPipeline.h" 


Tower::Tower()
{
}

Tower::~Tower()
{
}

TowersType Tower::getType() const
{
	return type;
}

int Tower::getLifePoints() const
{
	return getHealth();
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

TowerEmplacement Tower::getEmplacement() const
{
	return emplacement;
}

void Tower::onHealthChanged()
{
	setHealth(getHealth(), getMaxHealth());
}

void Tower::onDeath()
{
	notifyAllObservers(EventType::TowerDeactivated);
	deactivate();
}

