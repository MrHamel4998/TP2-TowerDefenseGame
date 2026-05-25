#pragma once
#include "ShootingTower.h"
#include "AnimatedObject.h"

class MageTower : public ShootingTower, public AnimatedObject
{
public:
	static constexpr int RECTANGLE_SIZE_X = 150;
	static constexpr int RECTANGLE_SIZE_Y = 150;

	MageTower();
	bool init() override;
	void update(const float deltaTime) override;
	void onShoot();

private:
	static constexpr int ANIMATION_COUNT = 1;
};