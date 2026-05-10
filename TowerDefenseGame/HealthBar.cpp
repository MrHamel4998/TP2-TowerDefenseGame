#include "HealthBar.h"

HealthBar::HealthBar()
{
}

HealthBar::~HealthBar()
{
    if (redBar != nullptr) delete redBar;
    if (greenBar != nullptr) delete greenBar;
}

void HealthBar::initHealthBar(const Texture& redTexture, const Texture& greenTexture)
{
    if (redBar != nullptr) delete redBar;
    if (greenBar != nullptr) delete greenBar;

    redBar = new Sprite(redTexture);
    greenBar = new Sprite(greenTexture);

    redBar->setOrigin(Vector2f(0.0f, 0.0f));
    greenBar->setOrigin(Vector2f(0.0f, 0.0f));
}

void HealthBar::setHealth(int current, int max)
{
    currentHealth = current;
    maxHealth = max;

    if (greenBar != nullptr && maxHealth > 0)
    {
        float healthRatio = static_cast<float>(currentHealth) / static_cast<float>(maxHealth);
        greenBar->setScale(Vector2f(healthRatio, 1.0f));
    }
}

void HealthBar::setPosition(const Vector2f& position)
{
    this->position = position;

    if (redBar != nullptr)
    {
        redBar->setPosition(Vector2f(position.x + HEALTH_BAR_OFFSET_X, position.y + HEALTH_BAR_OFFSET_Y));
    }

    if (greenBar != nullptr)
    {
        greenBar->setPosition(Vector2f(position.x + HEALTH_BAR_OFFSET_X, position.y + HEALTH_BAR_OFFSET_Y));
    }
}

void HealthBar::draw(RenderWindow& renderWindow) const
{
    if (redBar != nullptr)
    {
        renderWindow.draw(*redBar);
    }

    if (greenBar != nullptr)
    {
        renderWindow.draw(*greenBar);
    }
}
