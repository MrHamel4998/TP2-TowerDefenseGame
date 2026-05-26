#include "MageTower.h"
#include "ContentPipeline.h"
#include "Constants.h"

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
	setProjectileType(ProjectileType::Blast);
	setRange(300.0f);
	
	if (!initAnimationsStructure(ANIMATION_COUNT, Vector2i(RECTANGLE_SIZE_X, RECTANGLE_SIZE_Y))) return false;
	GameObject::setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	GameObject::setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	if (!initAnimation(0, 3, 0.15f, AnimationType::Linear)) return false;
	setIdleImage(0, 0);

	GameObject::getHealthBar().initHealthBar(ContentPipeline::getInstance().getRedBarTexture(), ContentPipeline::getInstance().getGreenBarTexture());
	GameObject::setHealth(getHealth(), getMaxHealth());

	return true;
}

void MageTower::onShoot()
{
	setActiveAnimation(0, true);
}

void MageTower::update(const float deltatime)
{
	updateAnimation(deltatime);

	if (isCurrentLinearAnimationIsOver())
	{
		setIdleImage(0, 0);
	}

	updateSpellEffects(deltatime);
}