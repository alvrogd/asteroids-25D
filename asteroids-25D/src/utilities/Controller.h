#ifndef CONTROLLER_H
#define	CONTROLLER_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// To modify the view matrix
#include "Shader.h"

// To be able to store references to the currently present objects in the scene
#include "../game-objects/asteroids/Asteroid.h"
#include "../game-objects/spaceship/Spaceship.h"
#include "../game-objects/visual-effects/ParticleSet.h"
#include "../game-objects/visual-effects/Particle.h"

// How many random asteroids are generated each time the game is restarted
#define RANDOM_ASTEROID_COUNT 100


class Controller
{
public:

	/* Attributes */

	// Window properties
	static int *wWidth;
	static int *wHeight;
	static float *aspectRatio;

	// Objects that are present in the scene
	static std::vector<Asteroid *> *asteroids;
	static std::vector<Particle *> *particles;
	static std::vector<ParticleSet *> *particleSets;

	// Multiple parameters related to the user input
	static bool fireButtonReleased;
	static int shotsPerFiring; // How many shots have been released during the current firing
	static bool resetButtonReleased;


	/* Getters & setters */

	static Spaceship *getSpaceship ()
	{
		return Controller::spaceship;
	}

	static void setSpaceship (Spaceship *spaceship)
	{
		Controller::spaceship = spaceship;

		// References to certain spaceship's fields are stored so that they can be directly referenced in the
		// controller's methods
		Controller::spaceshipPosition = Controller::spaceship->getPositionReference ();
		Controller::spaceshipSpeed = Controller::spaceship->getSpeedReference ();
		Controller::spaceshipRotation = Controller::spaceship->getRotationReference ();
		Controller::spaceshipRotationFix = Controller::spaceship->getRotationFixReference ();
		Controller::spaceshipAccelerationRate = Controller::spaceship->getAccelerationRateReference ();
	}


	/* Methods */

	// Camera control
	static void computeViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &cameraPosition);

	static void keyboardInput (GLFWwindow *window);

	static void resizeWindow (GLFWwindow *window, int width, int height);


private:

	/* Attributes */

	static int cameraMode;


	static Spaceship *spaceship;

	static glm::vec3 *spaceshipPosition;

	static glm::vec3 *spaceshipSpeed;

	static glm::vec3 *spaceshipRotation;

	static glm::vec3 *spaceshipRotationFix;

	static glm::vec3 *spaceshipAccelerationRate;
};

#endif
