#pragma once
#include "IObserver.h"
#include "GameObject.h"
#include "Tower.h"

class TowerEmplacement : public GameObject, public IObserver
{
public:
	TowerEmplacement();
	bool init();
	virtual ~TowerEmplacement() = default;

	void notify(Subject* subject, EventType eventType) override;
	bool isOccupied() const;
	void placeTower(Tower* tower);
	Tower* getTower() const;

private:
	Tower* tower = nullptr;
};