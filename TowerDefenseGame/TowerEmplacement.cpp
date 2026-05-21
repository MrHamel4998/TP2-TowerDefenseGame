#include "TowerEmplacement.h"
#include "Subject.h"
#include "Constants.h"
#include "ContentPipeline.h"

TowerEmplacement::TowerEmplacement()
{
}

bool TowerEmplacement::init()
{
	setTexture(ContentPipeline::getInstance().getTowerEmplacementTexture());
	return true;
}

void TowerEmplacement::notify(Subject* subject, EventType eventType)
{
	if (eventType == EventType::TowerDeactivated && subject == dynamic_cast<Subject*>(tower))
	{
		tower = nullptr;
		activate();
	}
}

bool TowerEmplacement::isOccupied() const
{
	return tower != nullptr;
}

void TowerEmplacement::placeTower(Tower* tower)
{
	if (!isOccupied())
	{
		this->tower = tower;
		deactivate();
	}
}

Tower* TowerEmplacement::getTower() const
{
	return tower;
}
