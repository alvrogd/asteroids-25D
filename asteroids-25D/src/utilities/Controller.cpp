#include "Controller.h"

// Window properties
int *Controller::wWidth = NULL;
int *Controller::wHeight = NULL;
float *Controller::aspectRatio = NULL;

// Objects that are present in the scene
std::vector<Asteroid *> *Controller::asteroids = NULL;
std::vector<Particle *> *Controller::particles = NULL;
std::vector<ParticleSet *> *Controller::particleSets = NULL;

// References to some spaceship's attributes
Spaceship *Controller::spaceship = NULL;
glm::vec3 *Controller::spaceshipPosition = NULL;
glm::vec3 *Controller::spaceshipSpeed = NULL;
glm::vec3 *Controller::spaceshipRotation = NULL;
glm::vec3 *Controller::spaceshipRotationFix = NULL;
glm::vec3 *Controller::spaceshipAccelerationRate = NULL;

// Multiple parameters related to the user input
bool Controller::fireButtonReleased = true;
int Controller::shotsPerFiring = 0;
bool Controller::resetButtonReleased = true;

// Camera mode
int Controller::cameraMode = 1;


// Camera control
void Controller::computeViewMatrix (glm::mat4 &viewMatrix, glm::vec3 &cameraPosition)
{
	// The camera is set behind the spaceship, as well as above it
	switch (Controller::cameraMode)
	{
		// Near view
		case 0:
			viewMatrix = glm::lookAt (
				glm::vec3 (spaceshipPosition->x + sinf (glm::radians (spaceshipRotation->y)) * 60.0f,
					20.0f,
					spaceshipPosition->z + cosf (glm::radians (spaceshipRotation->y)) * 60.0f),
				glm::vec3 (spaceshipPosition->x, spaceshipPosition->y, spaceshipPosition->z),
				glm::vec3 (0.0f, 1.0f, 0.0f));
			break;

		// Far view
		case 1:
		default:
			viewMatrix = glm::lookAt (
				glm::vec3 (spaceshipPosition->x + sinf (glm::radians (spaceshipRotation->y)) * 300.f,
					160.0f,
					spaceshipPosition->z + cosf (glm::radians (spaceshipRotation->y)) * 300.0f),
				glm::vec3 (spaceshipPosition->x, spaceshipPosition->y, spaceshipPosition->z),
				glm::vec3 (0.0f, 1.0f, 0.0f));
			break;
	}
}

void Controller::keyboardInput (GLFWwindow *window)
{
	// ESC = quit game
	if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose (window, true);
	}

	// Actions that the user can only perform if the spaceship is still alive
	if (!(spaceship->isDestroyed ()))
	{
		// W = speed up
		if (glfwGetKey (window, GLFW_KEY_W) == GLFW_PRESS)
		{
			// The spaceship is granted with speed depending on which direction it is facing
			spaceshipSpeed->x -= sinf (glm::radians (spaceshipRotation->y)) * spaceshipAccelerationRate->x;
			spaceshipSpeed->z -= cosf (glm::radians (spaceshipRotation->y)) * spaceshipAccelerationRate->z;

			spaceship->setSpeedingUp (true);
		}

		// Otherwise, the spaceship starts to slow down
		else
		{
			spaceship->setSpeedingUp (false);
		}

		// A = turn left
		if (glfwGetKey (window, GLFW_KEY_A) == GLFW_PRESS)
		{
			spaceshipRotation->y += 2.0f;
		}

		// D = turn right
		else if (glfwGetKey (window, GLFW_KEY_D) == GLFW_PRESS)
		{
			spaceshipRotation->y -= 2.0f;
		}

		// Spacebar = shoot
		if (glfwGetKey (window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			// Any of the following two conditions must be met in order to fire:
			//  - The spacebar was previously released
			//  - The maximum amount of shots per firing has not been reached yet
			if (Controller::fireButtonReleased || Controller::shotsPerFiring < 4)
			{
				Controller::spaceship->shoot ();
				Controller::fireButtonReleased = false;
				Controller::shotsPerFiring++;
			}
		}

		// Spacebar released
		else
		{
			Controller::fireButtonReleased = true;
			Controller::shotsPerFiring = 0;
		}
	}

	// R(eset) = reset game
	if (glfwGetKey (window, GLFW_KEY_R) == GLFW_PRESS)
	{
		// This check is needed in order to just perform one reset per R(eset) key press
		if (Controller::resetButtonReleased)
		{
			// All particles in the scene are deleted (the vector cannot be just "cleared" as the particles' memory
			// would not be released)
			while (Controller::particles->size () > 0)
			{
				delete Controller::particles->at (0);
			}

			// Same actions for the remaining objects in the scene
			while (Controller::particleSets->size () > 0)
			{
				delete Controller::particleSets->at (0);
			}

			while (Controller::asteroids->size () > 0)
			{
				delete Controller::asteroids->at (0);
			}

			// New 100 random asteroids are generated
			for (int i = 0; i < RANDOM_ASTEROID_COUNT; i++)
			{
				Controller::asteroids->push_back (new Asteroid ());
			}

			// The spaceship is set to visible
			spaceship->setDestroyed (false);

			// And its current shot count must also be reset
			Controller::shotsPerFiring = 0;

			Controller::resetButtonReleased = false;
		}
	}

	// R(eset) released
	else
	{
		Controller::resetButtonReleased = true;
	}

	// F1 = near camera
	if (glfwGetKey (window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		Controller::cameraMode = 0;
	}

	// F2 = far camera
	else if(glfwGetKey (window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		Controller::cameraMode = 1;
	}
}

void Controller::resizeWindow (GLFWwindow *window, int width, int height)
{
	// Where the new viewport's dimensions will be stored
	int renderWidth;
	int renderHeight;

	// If there is more available width than the requested height, the latter is the constraint
	if ((float)width / (float)height > *(Controller::aspectRatio))
	{
		renderHeight = height;
		renderWidth = (int) (*(Controller::aspectRatio) * renderHeight);
	}

	// If there is more available height than the requested width, the latter is the constraint
	else if ((float)width / (float)height < *(Controller::aspectRatio))
	{
		renderWidth = width;
		renderHeight = (int) (renderWidth / *(Controller::aspectRatio));
	}

	// Otherwise, the aspect ratio is preserved
	else
	{
		renderWidth = width;
		renderHeight = height;
	}

	// The viewport is updated bearing in mind any possible free space that could remain in height and width
	glViewport ((int) ((float)(width - renderWidth) / 2), (int) ((float)(height - renderHeight) / 2), renderWidth, renderHeight);

	// The new window size is stored
	*(Controller::wWidth) = width;
	*(Controller::wHeight) = height;
}
