#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Game
{
public:
	Game();
	int run();
	void reset();

	bool isShortMode() const;
	void setShortMode(bool value);

	bool isVictory() const;
	void setVictory(bool value);

	bool isGameOver() const;
	void setGameOver(bool value);

	int getCurrentWave() const;
	void nextWave();
	int getMaxWaves() const;

	int getScore() const;
	void addScore(int value);
	void setScore(int value);

private:
	RenderWindow renderWindow;
	Image icon;
	bool shortMode = false;
	bool victory = false;
	bool gameOver = false;
	int currentWave = 1;
	int score = 0;
};
