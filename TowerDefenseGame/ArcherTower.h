#pragma once
#include "ShootingTower.h"

class ArcherTower : public ShootingTower
{
public:
	static constexpr int PRICE = 70;

	ArcherTower();
	bool init() override;
	void update(const float deltaTime) override;
	int getPrice() const override;

private:
	int attackDamage;
	bool isShooting;
};