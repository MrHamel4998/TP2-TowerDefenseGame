#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Waypoint
{
public:
	Waypoint();
	Waypoint(const Vector2f& position);

	Waypoint* getNextWaypoint() const;
	void setNextWaypoint(Waypoint* nextWaypoint);
	Vector2f getPosition() const;

private:
	Vector2f position;
	Waypoint* nextWaypoint = nullptr;
};

