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
	   setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	   setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	   if (!initAnimation(FLY, 5, 0.1f, AnimationType::Circular)) return false;
	   if (!initAnimation(DEATH, 5, 0.1f, AnimationType::Linear)) return false;
	   setActiveAnimation(FLY, true);
	   return true;
}

void Demon::spawn(const Vector2f& position, Waypoint* firstWaypoint, int waveNumber)
{
	setTexture(ContentPipeline::getInstance().getDemonTexture());
	setOrigin(Vector2f(RECTANGLE_SIZE_X / 2.0f, RECTANGLE_SIZE_Y / 2.0f));
	setCollisionCircleRadius(RECTANGLE_SIZE_Y / 2.0f);
	setActiveAnimation(FLY, true);
	setPosition(position);
	this->currentTargetWaypoint = firstWaypoint;
	this->health = BASE_HEALTH;
	this->maxHp = BASE_HEALTH;
	this->speed = (0.9f + 0.1f * waveNumber) * 60.0f;
	activate();
}

void Demon::update(float deltaTime)
{
   // Si en train de mourir, avancer l'animation DEATH et se désactiver à la fin
   if (isDying)
   {
	   updateAnimation(deltaTime);
	   if (isCurrentLinearAnimationIsOver())
	   {
		   deactivate();
	   }
	   return;
   }

   // Si la vie est nulle ou négative, démarrer l'animation de mort
   if (health <= 0)
   {
	   setActiveAnimation(DEATH, true);
	   isDying = true;
	   updateAnimation(deltaTime);
	   return;
   }

   updateAnimation(deltaTime);

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

	// Flip le démon en fonction de la direction du waypoint
	if (currentTargetWaypoint != nullptr)
	{
		Vector2f currentPos = getPosition();
		Vector2f targetPos = currentTargetWaypoint->getPosition();

		if (targetPos.x > currentPos.x)
			setScale(Vector2f(1.0f, 1.0f));    // Waypoint à droite
		else
			setScale(Vector2f(-1.0f, 1.0f));   // Waypoint à gauche
	}
}