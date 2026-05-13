#pragma once
#include "IObserver.h"

class TowerEmplacement : public IObserver
{
public:
	TowerEmplacement();
	virtual ~TowerEmplacement() = default;

	void notify(Subject* subject, EventType eventType) override;

private:
};

