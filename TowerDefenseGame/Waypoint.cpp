#include "Waypoint.h"

Waypoint::Waypoint()
{

}

Waypoint::Waypoint(const Vector2f& position)
{
	this->position = position;
}

Waypoint* Waypoint::getNextWaypoint() const
{
	return this->nextWaypoint;
}

void Waypoint::setNextWaypoint(Waypoint* nextWaypoint)
{
	this->nextWaypoint = nextWaypoint;
}

Vector2f Waypoint::getPosition() const
{
	return this->position;
}