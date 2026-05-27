#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionCircle.h"
#include "HealthBar.h"

using namespace sf;

class GameObject : public Sprite
{
public:
	GameObject();
	GameObject(const Texture& texture);
	void setTexture(const Texture& texture);

	void activate();
	void deactivate();
	bool isActive() const;

	void setPosition(const float x, const float y);
	void setPosition(const Vector2f& position);

	const CollisionCircle& getCollisionCircle() const;
	float getCollisionCircleRadius() const;
	void setCollisionCircleRadius(const float radius);
	void useHorizontalAxisForCircleCollision();
	void useVerticalAxisForCircleCollision();
	bool isCircleColliding(const GameObject& other) const;

	HealthBar& getHealthBar();
	void setHealth(int current, int max);

	void move(const float offsetX, const float offsetY);
	void move(const Vector2f& offset);
	void moveBack();

	virtual void draw(RenderWindow& renderWindow) const;

private:
	bool active = false;
	CollisionCircle collisionCircle;
	HealthBar healthBar;
	Vector2f backupPosition;

	static Texture emptyTexture;
};