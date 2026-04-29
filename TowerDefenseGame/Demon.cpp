#include "Demon.h"
#include "ContentPipeline.h"

Demon::Demon()
{
	   setTexture(ContentPipeline::getInstance().getDemonTexture());
	   init();
}

bool Demon::init()
{
	   if (!initAnimationsStructure(ANIMATION_COUNT, Vector2i(RECTANGLE_SIZE_X, RECTANGLE_SIZE_Y))) return false;
	   if (!initAnimation(FLY, 5, 0.1f, AnimationType::Linear)) return false;
	   if (!initAnimation(DEATH, 5, 0.1f, AnimationType::Linear)) return false;
	   setActiveAnimation(FLY, true);
	   setIdleImage(FLY, 0);
	   setIdleImage(DEATH, 0);
	   return true;
}

void Demon::spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber)
{
	setPosition(position);
	setTexture(ContentPipeline::getInstance().getDemonTexture());
	this->currentTargetWaypoint = firstWaypoint;
	this->health = BASE_HEALTH;
	this->maxHp = BASE_HEALTH;
	this->speed = (0.9f + 0.1f * waveNumber) * 60.0f;
	activate();
}

void Demon::update(float deltaTime)
{
	if (currentTargetWaypoint == nullptr) return;

	Vector2f currentPos = getPosition();
	Vector2f targetPos = currentTargetWaypoint->getPosition();
	Vector2f direction = targetPos - currentPos;
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 2.0f)
	{
		currentTargetWaypoint = currentTargetWaypoint->getNextWaypoint();
		if (currentTargetWaypoint == nullptr)
		{
			deactivate();
			return;
		}
		return;
	}

	if (distance > 0.0f)
	{
		direction /= distance;
		float movement = speed * deltaTime;
		move(direction * movement);
	}
}