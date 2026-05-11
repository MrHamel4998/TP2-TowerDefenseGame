#pragma once
#include "Tower.h"


class ArcherTower : public Tower
{
public:
	void init() override;
	void update(const float deltaTime);

private:
	int attackDamage;
	bool isShooting;
};