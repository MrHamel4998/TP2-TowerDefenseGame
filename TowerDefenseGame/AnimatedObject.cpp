#include "AnimatedObject.h"

/// <summary>
/// Actuellement, si un AnimatedObject n'a pas �t� correctement initialis�, ce destructeur fera crasher l'application
/// �a ou un crash par Assert, �a s'�quivaut pas mal.
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
/// <param name="numberOfAnimations">Doit id�alement correspondre au nombre de lignes sur la spritesheet</param>
/// <param name="frameSize">Consid�re que la spritesheet dispose de frame d'animation r�guliers, sinon voir la m�thode adjustFrame</param>
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
/// Permet d'initialiser une animation donn�e
/// </summary>
/// <param name="animationNumber">Num�ro de la ligne sur la spritesheet (la premi�re est 0)</param>
/// <param name="numberOfFrames">Nombre de frames pour cette animation sur la spritesheet</param>
/// <param name="animationSpeed">Nombre de secondes que va durer chaque frame sur cette animation</param>
/// <param name="animationType">L'animation est de quel type? Lin�air, cyclique ou en pendule?</param>
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
/// Mise � jour de l'animation suivant les r�gles construites durant l'impl�mentation.
/// </summary>
/// <param name="deltaTime">Temps �coul� depuis le dernier rafraichissement d'�cran</param>
int AnimatedObject::updateAnimation(const float deltaTime)
{
	//� consid�rer seulement pour la dynamique ou on utilise la m�thode setIdleImage.
	//Si on ne l'utilise pas, ce bloc de code ne sera jamais utilis�.
	if (wasIdle)
	{
		timer = 0.0f;
		currentFrame = 0;
		direction = 1;
		setTextureRect(frames[currentAnimation][currentFrame]);
		wasIdle = false;
	}

	timer += deltaTime;

	if (timer >= animationSpeeds[currentAnimation]) //� chaque fois que le temps �galise ou d�passe notre temps d'animation
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
					currentFrame = numbersOfFrames[currentAnimation] - 1;  //En lin�aire on remet toujours le m�me rectangle
					currentLinearAnimationIsOver = true;
				}
				else
				{
					currentFrame = 0;
				}
			}
		}

		setTextureRect(frames[currentAnimation][currentFrame]);

		//Parfois �a peut-�tre pratique de savoir � quel frame on est rendu
		//L'Animation par contre c'est beaucoup plus clair
		return currentFrame;
	}

	return currentFrame;
}

/// <summary>
/// N�cessaire si notre image idle est une image quelconque sur la spriteSheet
/// Une fois que l'image idle est d�termin�e, updateAnimation ne devra pas �tre appell�
/// tant qu'on voudra que notre personnage reste idle.
/// </summary>
/// <param name="animationNumber">Le num�ro de l'animation o� se trouve notre image idle</param>
/// <param name="idleFrameNumber">Le num�ro du frame o� se trouve notre image idle</param>
void AnimatedObject::setIdleImage(const int animationNumber, const int idleFrameNumber)
{
	wasIdle = true;

	currentAnimation = animationNumber;

	setTextureRect(frames[currentAnimation][idleFrameNumber]);
}

/// <summary>
/// N�cessaire si notre image idle est une image quelconque sur la spriteSheet
/// Une fois que l'image idle est d�termin�e, updateAnimation ne devra pas �tre appell�
/// tant qu'on voudra que notre personnage reste idle.
/// </summary>
/// <param name="animationNumber">Le num�ro de l'animation o� se trouve notre image idle</param>
/// <param name="idleFrameNumber">Le num�ro du frame o� se trouve notre image idle</param>
bool AnimatedObject::ajustFrame(const int animationNumber, const int animationFrame, Vector2i framePosition, Vector2i frameSize)
{
	if (animationNumber >= numberOfAnimations || animationNumber < 0) return false;
	if (animationFrame >= numbersOfFrames[animationNumber] || animationFrame < 0) return false;

	frames[animationNumber][animationFrame].position = framePosition;
	frames[animationNumber][animationFrame].size = frameSize;

	return true;
}

bool AnimatedObject::isCurrentLinearAnimationIsOver()
{
	return currentLinearAnimationIsOver;
}