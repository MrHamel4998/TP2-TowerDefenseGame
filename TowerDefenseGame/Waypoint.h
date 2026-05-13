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
	
	Waypoint* getAlternativeWaypoint() const;
	void setAlternativeWaypoint(Waypoint* alternativeWaypoint);
	
	Vector2f getPosition() const;
	bool hasAlternative() const;

private:
	Vector2f position;
	Waypoint* nextWaypoint = nullptr;
	Waypoint* alternativeWaypoint = nullptr;
};

