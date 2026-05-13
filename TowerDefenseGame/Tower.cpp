#include "Tower.h"
#include "ContentPipeline.h" 
#include "ArcherTower.h"
#include "MageTower.h"
#include "KingTower.h"


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
	return lifePoints;
}

void Tower::setLifePoints(const int desiredLifePoints)
{
	lifePoints = desiredLifePoints;
}

void Tower::heal(const int amount)
{
	lifePoints += amount;
}

void Tower::takeDamage(const int amount)
{
	lifePoints -= amount;
}

bool Tower::isDead() const
{
	return lifePoints <= 0;
}

TowerEmplacement Tower::getEmplacement() const
{
	return emplacement;
}