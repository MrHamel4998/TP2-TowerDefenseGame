#pragma once
#include "GameScene.h"

class Level2 : public GameScene
{
public:
	Level2(RenderWindow& renderWindow, Game* game = nullptr);

protected:
	Maps getMapId() const override;
	Vector2f getDemonSpawnPosition() const override;
	Vector2f getKingTowerPosition() const override;
	const Vector2f* getWaypointPositions() const override;
	int getWaypointPositionsCount() const override;
	const Vector2f* getTowerEmplacementPositions() const override;
	int getTowerEmplacementPositionsCount() const override;
	void configureWaypoints() override;

private:
	static const int WAYPOINT_COUNT = 14;
	static const int EMPLACEMENT_COUNT = 9;
	static const Vector2f WAYPOINT_POSITIONS[WAYPOINT_COUNT];
	static const Vector2f TOWER_EMPLACEMENT_POSITIONS[EMPLACEMENT_COUNT];
};
