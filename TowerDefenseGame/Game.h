#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Game
{
public:
	Game();
	int run();

private:
	RenderWindow renderWindow;

	Image icon;
};