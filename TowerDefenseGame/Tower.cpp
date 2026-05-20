#include "Tower.h"
#include "ContentPipeline.h" 
#include "ArcherTower.h"
#include "MageTower.h"
#include "KingTower.h"
#include "Spell.h"
#include <iostream>


Tower::Tower()
{
}

Tower::~Tower()
{
}

Tower* Tower::create(const TowersType type)
{
	switch (type)
	{
		case TowersType::ARCHER:
			return new ArcherTower();

		case TowersType::MAGE:
			return new MageTower();

		case TowersType::KING:
			return new KingTower();
	}
	return nullptr;
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
	std::cout << "Tower took " << amount << " damage." << std::endl;
	std::cout << "Tower health before damage: " << getHealth() << std::endl;
	std::cout << "Tower health after damage: " << std::max(getHealth() - amount, 0) << std::endl;
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