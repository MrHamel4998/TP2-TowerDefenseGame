#include "Level2.h"

const Vector2f Level2::WAYPOINT_POSITIONS[WAYPOINT_COUNT] = {
	Vector2f(88.f, 412.f),
	Vector2f(168.f, 465.f),
	Vector2f(222.f, 588.f),
	Vector2f(308.f, 670.f),
	Vector2f(424.f, 668.f),
	Vector2f(510.f, 590.f),
	Vector2f(478.f, 468.f),
	Vector2f(516.f, 380.f),
	Vector2f(594.f, 360.f),
	Vector2f(806.f, 368.f),
	Vector2f(1140.f, 450.f),
	Vector2f(660.f, 598.f),
	Vector2f(804.f, 650.f),
	Vector2f(1140.f, 680.f)
};

const Vector2f Level2::TOWER_EMPLACEMENT_POSITIONS[Level2::EMPLACEMENT_COUNT] = {
	Vector2f(110, 620),
	Vector2f(228, 320),
	Vector2f(444, 780),
	Vector2f(362, 530),
	Vector2f(610, 222),
	Vector2f(998, 270),
	Vector2f(630, 460),
	Vector2f(935, 520),
	Vector2f(798, 760)
};

Level2::Level2(RenderWindow& renderWindow, Game* game)
	: GameScene(renderWindow, game)
{
}

Maps Level2::getMapId() const
{
	return Maps::Map2;
}

Vector2f Level2::getDemonSpawnPosition() const
{
	return Vector2f(-100, 410);
}

Vector2f Level2::getKingTowerPosition() const
{
	return Vector2f(1138, 564);
}

const Vector2f* Level2::getWaypointPositions() const
{
	return WAYPOINT_POSITIONS;
}

int Level2::getWaypointPositionsCount() const
{
	return WAYPOINT_COUNT;
}

const Vector2f* Level2::getTowerEmplacementPositions() const
{
	return TOWER_EMPLACEMENT_POSITIONS;
}

int Level2::getTowerEmplacementPositionsCount() const
{
	return EMPLACEMENT_COUNT;
}

void Level2::configureWaypoints()
{
	for (int i = 0; i < 5; i++)
	{
		waypoints[i]->setNextWaypoint(waypoints[i + 1]);
	}

	waypoints[5]->setNextWaypoint(waypoints[6]);
	waypoints[5]->setAlternativeWaypoint(waypoints[11]);

	for (int i = 6; i < 10; i++)
	{
		waypoints[i]->setNextWaypoint(waypoints[i + 1]);
	}

	for (int i = 11; i < 13; i++)
	{
		waypoints[i]->setNextWaypoint(waypoints[i + 1]);
	}
}
