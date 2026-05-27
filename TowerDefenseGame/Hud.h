#pragma once
#include <SFML/Graphics.hpp>
#include "string.h"

using namespace sf;

class Hud
{
public:
	Hud() {}
	~Hud();
	void hudInit(const Texture& hudMaskTexture, const Font& font);
	void draw(sf::RenderWindow& renderWindow);
	void update(int mana, int score, int kills, int wave, int highScore, bool archerSelected, bool mageSelected, bool sacredLightSelected, bool plagueSelected, bool isPaused);
	void setSpecialStateText(const String& text);

private:
	static const int INSTRUCTIONS_NUMBER = 6;
	static const float TEXT_POSITION_X;// = 1250;

	Text* specialStateText;

	Text* manaText;
	Text* scoreText;
	Text* killsText;

	Text* instructionTexts[INSTRUCTIONS_NUMBER] = {};

	Text* highScoreText;
	Text* waveText;

	Sprite* hudMask;
};