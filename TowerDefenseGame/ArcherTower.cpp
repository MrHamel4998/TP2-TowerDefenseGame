#include "ArcherTower.h"
#include "ContentPipeline.h"
#include "Constants.h"

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
	setFireRate(0.8f);
	setProjectileType(ProjectileType::Arrow);
	setPrice(PRICE);

	GameObject::getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());
	GameObject::setHealth(getHealth(), getMaxHealth());

	setAttackSound(ContentPipeline::getInstance().getArcherAttackSoundBuffer());

	return true;
}

void ArcherTower::update(const float deltaTime)
{
	updateSpellEffects(deltaTime);
}

int ArcherTower::getPrice() const
{
	return PRICE;
}