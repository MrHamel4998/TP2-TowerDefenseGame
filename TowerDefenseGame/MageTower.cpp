#include "MageTower.h"
#include "ContentPipeline.h"

MageTower::MageTower()
{
	GameObject::setTexture(ContentPipeline::getInstance().getMageTowerTexture());
	init();
}

bool MageTower::init()
{
	setType(TowersType::MAGE);
	setLifePoints(250);
	setAttackDamage(25);
	setFireRate(1.5f);
	
	if (!initAnimationsStructure(ANIMATION_COUNT, Vector2i(RECTANGLE_SIZE_X, RECTANGLE_SIZE_Y))) return false;
	GameObject::setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	GameObject::setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	if (!initAnimation(0, 3, 0.15f, AnimationType::Circular)) return false;
	setIdleImage(0, 0);
	setActiveAnimation(0);

	GameObject::getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());

	return true;
}

void MageTower::update(const float deltatime)
{
	updateAnimation(deltatime);
}