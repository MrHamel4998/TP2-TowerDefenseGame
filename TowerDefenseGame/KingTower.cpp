#include "KingTower.h"
#include "ContentPipeline.h"

KingTower::KingTower()
{
	GameObject::setTexture(ContentPipeline::getInstance().getKingTowerTexture());
	init();
}

bool KingTower::init()
{
	setType(TowersType::KING);
	setLifePoints(500);
	setCollisionCircleRadius(50);

	GameObject::getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());

	return true;
}

void KingTower::update(const float deltaTime)
{
}
