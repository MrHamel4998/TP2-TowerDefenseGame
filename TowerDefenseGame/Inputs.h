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
		mouseLeftButtonClicked = false;
		showWaypoints = false;
		leftMousePressed = false;
		plagueSelected = false;
		sacredLightSelected = false;
		mageTowerSelected = false;
		archerTowerSelected = false;
		pausePressed = false;
		enterPressed = false;
		escapePressed = false;
		sPressed = false;
	}

	Vector2f mousePosition;
	bool mouseLeftButtonClicked;
	bool showWaypoints;
	bool leftMousePressed;
	bool plagueSelected;
	bool sacredLightSelected;
	bool mageTowerSelected;
	bool archerTowerSelected;
	bool pausePressed;
	bool enterPressed;
	bool escapePressed;
	bool sPressed;
};
