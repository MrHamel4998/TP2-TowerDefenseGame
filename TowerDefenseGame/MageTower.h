#pragma once
#include "ShootingTower.h"
#include "AnimatedObject.h"

class MageTower : public ShootingTower, AnimatedObject
{
public:
	static constexpr int RECTANGLE_SIZE_X = 100;
	static constexpr int RECTANGLE_SIZE_Y = 50;

	MageTower();
	bool init() override;
	void update(const float deltaTime) override;

private:
	const int ANIMATION_COUNT = 1;
};