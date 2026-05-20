#pragma once
#include <SFML/Graphics.hpp>

struct Inputs
{
	Inputs()
	{
		reset();
		plagueSelected = true; // juste pour le test, à enlever lorsque la mécanique du HUD de sélection des sorts sera implémentée
	}

	void reset()
	{
		mousePosition = Vector2f(0.0f, 0.0f);
		mouseLeftButtonClicked = false;
		showWaypoints = false;
	}

	Vector2f mousePosition;
	bool mouseLeftButtonClicked;
	bool showWaypoints;
	bool leftMousePressed;
	bool plagueSelected;
	bool sacredLightSelected;
};
