#include "Level1.h"

const Vector2f Level1::WAYPOINT_POSITIONS[WAYPOINT_COUNT] = {
	Vector2f(610, 8),
	Vector2f(630, 222),
	Vector2f(595, 444),
	Vector2f(478, 514),
	Vector2f(320, 558),
	Vector2f(260, 620),
	Vector2f(280, 720),
	Vector2f(348, 812),
	Vector2f(526, 862),
	Vector2f(720, 830),
	Vector2f(968, 850),
	Vector2f(1110, 682)
};

const Vector2f Level1::TOWER_EMPLACEMENT_POSITIONS[EMPLACEMENT_COUNT] = {
	Vector2f(470, 170),
	Vector2f(770, 250),
	Vector2f(440, 370),
	Vector2f(650, 520),
	Vector2f(120, 650),
	Vector2f(470, 700),
	Vector2f(850, 710),
	Vector2f(660, 950)
};

Level1::Level1(RenderWindow& renderWindow, Game* game)
	: GameScene(renderWindow, game)
{
}

Maps Level1::getMapId() const
{
	return Maps::Map1;
}

Vector2f Level1::getDemonSpawnPosition() const
{
	return Vector2f(610, -100);
}

Vector2f Level1::getKingTowerPosition() const
{
	return Vector2f(1138, 600);
}

const Vector2f* Level1::getWaypointPositions() const
{
	return WAYPOINT_POSITIONS;
}

int Level1::getWaypointPositionsCount() const
{
	return WAYPOINT_COUNT;
}

const Vector2f* Level1::getTowerEmplacementPositions() const
{
	return TOWER_EMPLACEMENT_POSITIONS;
}

int Level1::getTowerEmplacementPositionsCount() const
{
	return EMPLACEMENT_COUNT;
}

void Level1::configureWaypoints()
{
	for (int i = 0; i < waypointCount - 1; i++)
	{
		waypoints[i]->setNextWaypoint(waypoints[i + 1]);
	}
}
