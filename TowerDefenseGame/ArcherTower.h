#pragma once
#include "ShootingTower.h"


class ArcherTower : public ShootingTower
{
public:
	ArcherTower();
	bool init() override;
	void update(const float deltaTime) override;

private:
	int attackDamage;
	bool isShooting;
};