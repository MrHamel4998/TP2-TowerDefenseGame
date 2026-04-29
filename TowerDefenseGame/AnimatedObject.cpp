#include "AnimatedObject.h"

/// <summary>
/// Actuellement, si un AnimatedObject n'a pas été correctement initialisé, ce destructeur fera crasher l'application
/// ça ou un crash par Assert, ça s'équivaut pas mal.
/// </summary>
AnimatedObject::~AnimatedObject()
{
	for (int i = 0; i < numberOfAnimations; i++) 
		delete[] frames[i]; 

	delete[] frames;
	delete[] numbersOfFrames;
	delete[] animationSpeeds;
	delete[] animationTypes;
}

/// <summary>
/// Initialisation du nombre d'animations que notre acteur disposera et de la taille de chaque frame
/// </summary>
/// <param name="numberOfAnimations">Doit idéalement correspondre au nombre de lignes sur la spritesheet</param>
/// <param name="frameSize">Considère que la spritesheet dispose de frame d'animation réguliers, sinon voir la méthode adjustFrame</param>
/// <returns>Si l'initialisation s'est faite correctement ou non.</returns>

bool AnimatedObject::initAnimationsStructure(const int numberOfAnimations, const Vector2i frameSize)
{
	if (numberOfAnimations < 1) return false;

	this->numberOfAnimations = numberOfAnimations;
	this->frameSize = frameSize;

	frames = new IntRect*[numberOfAnimations];
	numbersOfFrames = new int[numberOfAnimations];
	animationSpeeds = new float[numberOfAnimations];
	animationTypes = new AnimationType[numberOfAnimations];

	return true;
}

/// <summary>
/// Permet d'initialiser une animation donnée
/// </summary>
/// <param name="animationNumber">Numéro de la ligne sur la spritesheet (la première est 0)</param>
/// <param name="numberOfFrames">Nombre de frames pour cette animation sur la spritesheet</param>
/// <param name="animationSpeed">Nombre de secondes que va durer chaque frame sur cette animation</param>
/// <param name="animationType">L'animation est de quel type? Linéair, cyclique ou en pendule?</param>
/// <returns>Si l'initialisation s'est faite correctement ou non.</returns>
bool AnimatedObject::initAnimation(const int animationNumber, const int numberOfFrames, const float animationSpeed, const AnimationType animationType)
{
	if (animationNumber >= numberOfAnimations || animationNumber < 0) return false;

	frames[animationNumber] = new IntRect[numberOfFrames];
	numbersOfFrames[animationNumber] = numberOfFrames;
	animationSpeeds[animationNumber] = animationSpeed;
	this->animationTypes[animationNumber] = animationType;

	for (int i = 0; i < numberOfFrames; i++)
	{
		frames[animationNumber][i].size = frameSize;
		frames[animationNumber][i].position = Vector2i(i * frameSize.x, animationNumber * frameSize.y);
	}

	return true;
}

void AnimatedObject::setActiveAnimation(const int animationNumber, const bool resetAnimation)
{
	this->currentAnimation = animationNumber;
	currentLinearAnimationIsOver = false;

	if (resetAnimation)
	{
		timer = 0.0f;
		currentFrame = 0;
		direction = 1;
	}

	setTextureRect(frames[currentAnimation][currentFrame]);
}

/// <summary>
/// Mise à jour de l'animation suivant les règles construites durant l'implémentation.
/// </summary>
/// <param name="deltaTime">Temps écoulé depuis le dernier rafraichissement d'écran</param>
int AnimatedObject::updateAnimation(const float deltaTime)
{
	//À considérer seulement pour la dynamique ou on utilise la méthode setIdleImage.
	//Si on ne l'utilise pas, ce bloc de code ne sera jamais utilisé.
	if (wasIdle)
	{
		timer = 0.0f;
		currentFrame = 0;
		direction = 1;
		setTextureRect(frames[currentAnimation][currentFrame]);
		wasIdle = false;
	}

	timer += deltaTime;

	if (timer >= animationSpeeds[currentAnimation]) //À chaque fois que le temps égalise ou dépasse notre temps d'animation
	{
		timer -= animationSpeeds[currentAnimation];

		currentFrame += direction;

		if (animationTypes[currentAnimation] == AnimationType::Pendulum)
		{
			if (currentFrame == 0 || currentFrame == (numbersOfFrames[currentAnimation] - 1))
				direction *=  -1;
		}
		else
		{
			if (currentFrame == numbersOfFrames[currentAnimation])
			{
				if (animationTypes[currentAnimation] == AnimationType::Linear)
				{
					currentFrame = numbersOfFrames[currentAnimation] - 1;  //En linéaire on remet toujours le même rectangle
					currentLinearAnimationIsOver = true;
				}
				else
				{
					currentFrame = 0;
				}
			}
		}

		setTextureRect(frames[currentAnimation][currentFrame]);

		//Parfois ça peut-être pratique de savoir à quel frame on est rendu
		//L'Animation par contre c'est beaucoup plus clair
		return currentFrame;
	}
}

/// <summary>
/// Nécessaire si notre image idle est une image quelconque sur la spriteSheet
/// Une fois que l'image idle est déterminée, updateAnimation ne devra pas être appellé
/// tant qu'on voudra que notre personnage reste idle.
/// </summary>
/// <param name="animationNumber">Le numéro de l'animation où se trouve notre image idle</param>
/// <param name="idleFrameNumber">Le numéro du frame où se trouve notre image idle</param>
void AnimatedObject::setIdleImage(const int animationNumber, const int idleFrameNumber)
{
	wasIdle = true;

	currentAnimation = animationNumber;

	setTextureRect(frames[currentAnimation][idleFrameNumber]);
}

/// <summary>
/// Nécessaire si notre image idle est une image quelconque sur la spriteSheet
/// Une fois que l'image idle est déterminée, updateAnimation ne devra pas être appellé
/// tant qu'on voudra que notre personnage reste idle.
/// </summary>
/// <param name="animationNumber">Le numéro de l'animation où se trouve notre image idle</param>
/// <param name="idleFrameNumber">Le numéro du frame où se trouve notre image idle</param>
bool AnimatedObject::ajustFrame(const int animationNumber, const int animationFrame, Vector2i framePosition, Vector2i frameSize)
{
	if (animationNumber >= numberOfAnimations || animationNumber < 0) return false;
	if (animationFrame >= numbersOfFrames[animationNumber] || animationFrame < 0) return false;

	frames[animationNumber][animationFrame].position = framePosition;
	frames[animationNumber][animationFrame].size = frameSize;
}

bool AnimatedObject::isCurrentLinearAnimationIsOver()
{
	return currentLinearAnimationIsOver;
}