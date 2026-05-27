#pragma once
#include <SFML/Graphics.hpp>

struct Inputs
{
	Inputs()
	{
		reset();
	}

	void reset()
	{
		mousePosition = Vector2f(0.0f, 0.0f);
		showWaypoints = false;
		leftMousePressed = false;
		plagueSelected = false;
		sacredLightSelected = false;
		mageTowerSelected = false;
		archerTowerSelected = false;
		pausePressed = false;
		enterPressed = false;
	}

	Vector2f mousePosition;
	bool showWaypoints;
	bool leftMousePressed;
	bool plagueSelected;
	bool sacredLightSelected;
	bool mageTowerSelected;
	bool archerTowerSelected;
	bool pausePressed;
	bool enterPressed;
};
