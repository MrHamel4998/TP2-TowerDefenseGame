#pragma once
#include "GameObject.h"

class AnimatedObject : virtual public GameObject
{
public:
	enum AnimationType {Linear, Circular, Pendulum};

	virtual ~AnimatedObject();
	bool initAnimationsStructure(const int numberOfAnimations, const Vector2i frameSize);
	bool initAnimation(const int animationNumber, const int numberOfFrames, const float animationSpeed, const AnimationType animationType);
	void setActiveAnimation(const int animationNumber, const bool resetAnimation = false);

	int updateAnimation(const float deltaTime);
	void setIdleImage(const int animationNumber, const int idleFrameNumber);

	bool ajustFrame(const int animationNumber, const int animationFrame, Vector2i framePosition, Vector2i frameSize);
	bool isCurrentLinearAnimationIsOver();

private:
	IntRect** frames = nullptr;

	int numberOfAnimations = -1;
	int currentAnimation = 0;
	int currentFrame = 0;
	
	Vector2i frameSize;
	
	float timer = 0.0f;
	int direction = 1;

	int* numbersOfFrames = nullptr;
	float* animationSpeeds = nullptr;
	AnimationType* animationTypes = nullptr;

	bool wasIdle = true;
	bool currentLinearAnimationIsOver = false;
};

