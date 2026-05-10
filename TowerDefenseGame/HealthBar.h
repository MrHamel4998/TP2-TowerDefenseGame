#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class HealthBar
{
public:
    HealthBar();
    ~HealthBar();

    void initHealthBar(const Texture& redTexture, const Texture& greenTexture);
    void setHealth(int current, int max);
    void setPosition(const Vector2f& position);
    void draw(RenderWindow& renderWindow) const;

private:
    static constexpr float HEALTH_BAR_WIDTH = 50.0f;
    static constexpr float HEALTH_BAR_OFFSET_X = -30.0f;
    static constexpr float HEALTH_BAR_OFFSET_Y = -30.0f;

    Sprite* redBar = nullptr;
    Sprite* greenBar = nullptr;

    Vector2f position;
    int currentHealth = 0;
    int maxHealth = 100;
};
