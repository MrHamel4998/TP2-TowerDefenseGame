#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

/*
À NOTER: La couleur Gold pour le texte de "l'action active" : Color(212, 175, 55, 255)
*/
class Hud
{
public:
	Hud() {}
	~Hud();
	void hudInit(const Texture& hudMaskTexture, const Font& font);
	void draw(sf::RenderWindow& renderWindow);
	void update(int mana, int score, int kills, int wave, int highScore, bool archerSelected, bool mageSelected, bool sacredLightSelected, bool plagueSelected);


private:
	static const int INSTRUCTIONS_NUMBER = 6;
	static const float TEXT_POSITION_X;// = 1250;

	bool endGame = false;

	Text* specialStateText;

	Text* manaText;
	Text* scoreText;
	Text* killsText;

	Text* instructionTexts[INSTRUCTIONS_NUMBER] = {};

	Text* highScoreText;
	Text* waveText;

	Sprite* hudMask;
};
