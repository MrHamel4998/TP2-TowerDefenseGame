#pragma once
#include "Tower.h"

class KingTower : public Tower
{
public:
	KingTower();
	bool init() override;
	void update(const float deltaTime) override;

};