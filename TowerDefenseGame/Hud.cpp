#include "Hud.h"

const float Hud::TEXT_POSITION_X = 1250.0f;

Hud::~Hud()
{
	if (specialStateText != nullptr) delete specialStateText;
	if (manaText != nullptr) delete manaText;
	if (scoreText != nullptr) delete scoreText;
	if (killsText != nullptr) delete killsText;
	if (highScoreText != nullptr) delete highScoreText;
	if (waveText != nullptr) delete waveText;
	if (hudMask != nullptr) delete hudMask;

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
		if (instructionTexts[i] != nullptr) delete instructionTexts[i];
}

void Hud::hudInit(const Texture& hudMaskTexture, const Font& font)
{
	hudMask = new Sprite(hudMaskTexture);

	specialStateText = new Text(font, "Instructions diverses ici!", 25U);
	specialStateText->setFillColor(Color::White);
	specialStateText->setPosition({ TEXT_POSITION_X, 10.0f });

	//Utilisation sans honte du constructeur de copie
	manaText = new Text(*specialStateText);
	scoreText = new Text(*specialStateText);
	killsText = new Text(*specialStateText);
	waveText = new Text(*specialStateText);
	highScoreText = new Text(*specialStateText);

	waveText->setPosition({TEXT_POSITION_X, 110.0f});
	manaText->setPosition({TEXT_POSITION_X, 210.0f});
	killsText->setPosition({TEXT_POSITION_X, 250.0f});
	scoreText->setPosition({TEXT_POSITION_X, 290.0f});
	highScoreText->setPosition({TEXT_POSITION_X, 330.0f});


	waveText->setString("Wave - 1");
	manaText->setString("Mana - 0");
	killsText->setString("Kills - 0");
	scoreText->setString("Score - 0");
	highScoreText->setString("HighScore - 0");

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
	{
		instructionTexts[i] = new Text(*specialStateText);
		instructionTexts[i]->setPosition({TEXT_POSITION_X, 430 + i * 40.0f});
	}

	instructionTexts[0]->setStyle(Text::Underlined);
	instructionTexts[0]->setString("Instructions");
	instructionTexts[1]->setString("Z: Create archer tower (70 manas)");
	instructionTexts[2]->setString("X: Create mage tower (100 manas)");
	instructionTexts[3]->setString("C: Plague spell (20 manas)");
	instructionTexts[4]->setString("V: Sacred light spell (60 manas)");
	instructionTexts[5]->setString("P: Pause");

	specialStateText->setCharacterSize(35U);
}

void Hud::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*hudMask);
	renderWindow.draw(*specialStateText);
	renderWindow.draw(*manaText);
	renderWindow.draw(*scoreText);
	renderWindow.draw(*killsText);
	renderWindow.draw(*highScoreText);
	renderWindow.draw(*waveText);

	for (int i = 0; i < INSTRUCTIONS_NUMBER; i++)
		renderWindow.draw(*instructionTexts[i]);
}

void Hud::update(int mana, int score, int kills, int wave, int highScore, bool archerSelected, bool mageSelected, bool sacredLightSelected, bool plagueSelected)
{
	manaText->setString("Mana - " + std::to_string(mana));
	scoreText->setString("Score - " + std::to_string(score));
	killsText->setString("Kills - " + std::to_string(kills));
	waveText->setString("Wave - " + std::to_string(wave));
	highScoreText->setString("HighScore - " + std::to_string(highScore));

	// Highlight de la sélection active
	Color gold = Color(212, 175, 55, 255);
	Color white = Color::White;

	instructionTexts[1]->setFillColor(archerSelected ? gold : white);
	instructionTexts[2]->setFillColor(mageSelected ? gold : white);
	instructionTexts[3]->setFillColor(plagueSelected ? gold : white);
	instructionTexts[4]->setFillColor(sacredLightSelected ? gold : white);
}