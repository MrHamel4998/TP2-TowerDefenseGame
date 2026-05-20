#include "ArcherTower.h"
#include "ContentPipeline.h"

ArcherTower::ArcherTower()
{
	setTexture(ContentPipeline::getInstance().getArcherTowerTexture());
	init();
}

bool ArcherTower::init()
{
	setType(TowersType::ARCHER);
	setLifePoints(250);
	setAttackDamage(10);
	setFireRate(0.0f);

	GameObject::getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());
	GameObject::setHealth(getHealth(), getMaxHealth());

	return true;
}

void ArcherTower::update(const float deltaTime)
{

}