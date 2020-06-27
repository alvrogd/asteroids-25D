// PI value
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// Windows API
#include <windows.h>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Sound library
#include <irrKlang.h>

// Multithreading
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>

// I/O
#include <iostream>

// Custom utilities
#include "Initializer.h" // OpenGL initializer
#include "Controller.h"// Input handler

// To create any custom object present in the game
#include "Model.h"
#include "Shader.h"
#include "LightSource.h"
#include "Shape.h"
#include "Object.h"
#include "Mobile.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include "Shot.h"
#include "Particle.h"
#include "ParticleSet.h"
#include "Cubemap.h"

// Custom utility to play sounds
#include "Sound.h"

#include <vector>
#include <ctime>


/* ===== Definitions about required external files ===== */

// Files that contain all shader programs
#define SH_VERTEX_MODELS "shader.vert"
#define SH_FRAGMENT_MODELS "shader.frag"
#define SH_VERTEX_COLOR "shaderColor.vert"
#define SH_FRAGMENT_COLOR "shaderColor.frag"
#define SH_VERTEX_SKYBOX "shaderSkybox.vert"
#define SH_FRAGMENT_SKYBOX "shaderSkybox.frag"
#define SH_VERTEX_PARTICLES "shaderParticle.vert"
#define SH_FRAGMENT_PARTICLES "shaderParticle.frag"

// How many light sources the main shader program supports at most
#define MAX_LIGHT_SOURCES 50

// Files that make up the skybox
std::vector<std::string> skyboxFaces
{
	"PositiveX.png",
	"NegativeX.png",
	"PositiveY.png",
	"NegativeY.png",
	"PositiveZ.png",
	"NegativeZ.png"
};

// Paths for game object models
#define MODEL_SPACESHIP "Viper-mk-IV-fighter.obj"
#define MODEL_ASTEROIDS "rock_by_dommk.obj"

// Music & sound effects
#define SOUND_BG_MUSIC "Galactic Funk.ogg"


/* ===== Global variables ===== */

// Shaders used during the rendering process
Shader *shader = NULL;
Shader *shaderColor = NULL;
Shader *shaderSkybox = NULL;
Shader *shaderParticles = NULL;


// Window & camera properties
GLFWwindow *window = NULL;
int wWidth = 800;
int wHeight = 800;
float aspectRatio = (float)wWidth / (float)wHeight;
const double cameraFOV = 80.0;
double maxRenderDistance = 100.0; // Objects that are farther that this are not rendered


// This program uses a main thread + an auxiliary one. The latter takes care of updating most present particles in the
// game scene in order to increase framerate by reducing the main thread's workload
//
// Mutexes and condition variables thorugh which condition barriers are emulated (not available in C++ when writing
// this code)
//
//	- 1st region: an auxiliary thread will wait for the main thread to start a new iteration of the main loop
//	- 2nd region: the main thread will wait for the aux. thread to update all particles
std::mutex mutexSection1;
std::mutex mutexSection2;
std::condition_variable conditionSection1;
std::condition_variable conditionSection2;

// Auxiliary variables that help synchronizing both threads
bool newDelta = false; // If a new frame is being rendered; that is, the aux. thread must perform another batch update
bool auxThreadIterationFinished = false; // If the aux. thread has already finishing the last batch update


// Elapsed time between frames (frame = iteration of the main loop)
float previousTime = 0;
float currentTime = 0;
float delta = 0;


// If the camera is shaking, and the timestamp when it last began
bool isShaking = false;
float lastShake = 0.0f;


// Objects rendered in the game scene, along with their models
Spaceship *spaceship = NULL;
LightSource *mainLightSource = NULL;
Cubemap *skybox = NULL;
std::vector<Asteroid *> asteroids;
std::vector<Shot *> shots;
std::vector<Particle *> particles;
std::vector<ParticleSet *> particleSets;

Model *spaceshipModel = NULL;
Model *asteroidsModel = NULL;



/* ===== Main functions ===== */

/**
 * Sets up (e.g., loads the spaceship's model, sets the mobile objects' warp range...) and initializes the main game
 * objects:
 *
 *	- Spaceship
 *	- Asteroids
 *	- Main light source
 *	- Skybox
 *
 * It also takes care of performing any needed initialization for game objects that are dinamically generated during
 * the gameplay, such as the spaceship's shots and the explosions' particles
 */
void initializeGameObjects();

/**
 * Destroys all game objects, along with their models
 */
void deleteGameObjects();

/**
 * Updates all game objects depending on the elapsed time from the last frame, and also renders a new one
 */
void mainLoop();

/**
 * Updates the state of all game objects depending on the elapsed time from the last frame
 */
void updateGameObjects();

/**
 * Fuction used by the main thread to let the aux. one know that it must proceed to update all present particles in
 * order to render a new frame
 */
void notifyAuxThread();

/**
 * Renders a new frame
 */
void display();

/*
 * Fuction used by the aux. thread to update the positions of all the currently shown particles; any particle whose
 * lifetime expires is also removed
 */
void updateParticleSets(std::future<void> exitCondition);


int main (int argc, char *argv[])
{
	// Time is used as a seed for the pseudo-random number generator
	std::srand (std::time (NULL));


	/* OpenGL initialization */

	Initializer::initializeOpenGL (&window, "Asteroids 2.5D", &wWidth, &wHeight, &aspectRatio);


	/* Shader program compilation */

	shader = new Shader(SH_VERTEX_MODELS, SH_FRAGMENT_MODELS);
	shaderColor = new Shader(SH_VERTEX_COLOR, SH_FRAGMENT_COLOR);
	shaderSkybox = new Shader(SH_VERTEX_SKYBOX, SH_FRAGMENT_SKYBOX);
	shaderParticles = new Shader(SH_VERTEX_PARTICLES, SH_FRAGMENT_PARTICLES);


	/* OpenGL OpenGL configuration */

	Initializer::configureOpenGL();


	/* Generation of all game objects */

	initializeGameObjects();
	   

	/* Final preparations & render loop (main loop) */

	// The auxiliary thread gets spawned (previously explained), along with an auxiliary variable through which the
	// main thread can command him to stop
	std::promise<void> stopSignal;
	std::future<void> future = stopSignal.get_future();

	std::thread particleSetsThread(updateParticleSets, std::move(future));

	// The background music starts to play in an infinite loop
	Sound::getSound()->getSound2D()->play2D(SOUND_BG_MUSIC, GL_TRUE);

	std::cout << std::endl << "========== Game successfully started! ==========" << std::endl << std::endl;

	// As long as the program is not signaled to stop
	while (!glfwWindowShouldClose (window))
	{
		mainLoop();
	}


	/* Clean-up in order to exit */

	// Music and all game objects are destroyed
	Sound::getSound()->~Sound();

	// The auxiliary thread is commanded to stop
	stopSignal.set_value ();
	// This thread could already be waiting for the main thread to let him know that it must perform another batch
	// update of the particles. Therefore, it is crucial to notify the thread just in case it is already sleeping,
	// waiting for that precise command
	newDelta = true;
	conditionSection1.notify_one ();

	// The main thread deletes all games objects while the auxiliary thread finishes its execution
	deleteGameObjects();

	// The aux. thread should have stopped by now
	particleSetsThread.join ();


	exit(EXIT_SUCCESS);
}

void initializeGameObjects()
{
	// Spheres will be needed for both shots and explosions
	Shape::initializeShapes();

	// 1000x1000 units area
	// THE MOBILE OBJECT'S WARP MUST BE ESTABLISHED BEFORE SPAWNING ANY ASTEROID
	Mobile::warpCoordinates = glm::vec3(2000.0f, 2000.0f, 2000.0f);

	// As the warp area is a square, the minimum render distance that is needed in order to allow the player to see
	// all objects from any position is the distance from a certain corner of the area to the opposite one
	maxRenderDistance = std::sqrt(2.0 * 4000.0 * 4000.0);

	// ÇAsteroids, shots and particles need to know where they are stored in order to remove themselves when dying
	Asteroid::asteroidSet = &asteroids;
	Shot::shotsSet = &shots;
	Particle::particleSet = &particles;
	ParticleSet::particleSetContainer = &particleSets;

	// Loading required models from disk
	spaceshipModel = new Model(MODEL_SPACESHIP);
	asteroidsModel = new Model(MODEL_ASTEROIDS);
	Asteroid::model = asteroidsModel;

	// The spaceship's model points initially to the -X direction, so a rotation fix is applied
	spaceship = new Spaceship(glm::vec3(1.0f, 1.0f, 1.0f), spaceshipModel, 8.0f, glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.02f, 0.02f, 0.02f),
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -90.0f, 0.0f));

	// As many random asteroids as desired are generated
	for (int i = 0; i < RANDOM_ASTEROID_COUNT; i++)
	{
		asteroids.push_back(new Asteroid());
	}

	// There is also a main light source that lights up the scene (mainly white)
	mainLightSource = new LightSource(glm::vec3(0.0f, 30.0f, 30.0f), 1.0f, 0.000028f, 0.00000014f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// And the skybox is needed so that it seems that the spaceship is in the middle of the space
	skybox = new Cubemap(skyboxFaces);


	// The controller also needs references to certain game objects in order to properly work
	Controller::asteroids = &asteroids;
	Controller::particles = &particles;
	Controller::particleSets = &particleSets;
	Controller::setSpaceship(spaceship);
}

void deleteGameObjects()
{
	Shape::destroyShapes();

	delete spaceship;
	delete spaceshipModel;

	while (particles.size() > 0)
	{
		delete particles.at(0); // A particle removes itself from the vector
	}

	while (particleSets.size() > 0)
	{
		delete particleSets.at(0); // A ParticleSet removes itself from the vector
	}

	while (asteroids.size() > 0)
	{
		delete asteroids.at(0); // An asteroid removes itself from the vector
	}

	delete asteroidsModel;

	delete mainLightSource;

	delete skybox;

	delete shader;
	delete shaderColor;
	delete shaderSkybox;
	delete shaderParticles;
}

void mainLoop()
{
	/* User input */

	Controller::keyboardInput(window);


	/* Game objects update */

	updateGameObjects();


	/* Render */

	display();
	glfwSwapBuffers(window); // Swaps the front and back buffers in order to display the new rendered frame


	/* Final chores */

	// Any remaining events are processed Se procesan eventos pendientes (window resizing...)
	glfwPollEvents();
}

void updateGameObjects()
{
	// The time elapsed since the GLFW library was loaded will be used to compute the elapsed time from the last
	// rendered frame, which will determine how the game objects are updated
	currentTime = (float)glfwGetTime();

	// The main thread lets the aux. one that it must perform another update of the position of the particles that are
	// contained in ParticleSets (= all explosions), whereas individual particles will be still updated by the main
	// thread
	notifyAuxThread();

	
	/* All the game objects apart from the particles in ParticleSets are updated by the main thread */

	if (!(spaceship->isDestroyed()))
	{
		spaceship->updateState(delta);
	}

	for (Asteroid *asteroid : asteroids)
	{
		asteroid->updateState(delta);
	}

	for (unsigned int i = 0; i < shots.size(); i++) // Cannot use "for each iterator" as it would break when removing
	{												// the current element
		Shot *shot = shots.at(i);

		shot->updateState(delta);

		if (shot->getRemainingWarps() < 0)
		{
			delete shot;
			i--;
		}
	}

	for (unsigned int i = 0; i < particles.size(); i++)
	{
		Particle *particle = particles.at(i);

		particle->updateState(delta);

		if (particle->isDead())
		{
			delete particle;
			i--;
		}
	}


	// The new spaceship's position also determines how the player hears any present sound effect; the direction in
	// which the player looks also alters the result
	glm::vec3 spaceshipPosition = spaceship->getPosition();

	glm::vec3 spaceshipDirection = glm::vec3(
		-sinf(glm::radians(spaceship->getRotation().y)),
		0.0f,
		-cos(glm::radians(spaceship->getRotation().y))
	);
	// cos^2(x) + sen^2(x) = 1 -> unit vector
	spaceshipDirection *= spaceshipDirection;

	Sound::getSound()->update(spaceshipPosition, spaceshipDirection);


	/* The spaceship, asteroids and shots are checked for collisions */

	// Checking if the spaceship has collided with any asteroid
	bool spaceshipHasCollided = false;
	Asteroid *collidingAsteroidWithSpaceship = NULL;

	if (!(spaceship->isDestroyed()))
	{
		for (unsigned int i = 0; i < asteroids.size(); i++)
		{
			if (spaceship->checkCollision(asteroids.at(i)))
			{
				collidingAsteroidWithSpaceship = asteroids.at(i); // The colliding asteroid is stored for later usage
				spaceshipHasCollided = true;

				break; // There is no need to check the spaceship against the remaining asteroids
			}
		}
	}

	// Now, the main thread will wait for the auxiliary one to stop its current batch update. The synchronization
	// between both threads is performed at this point because the main thread will now check a bunch of collisions
	// that may produce explosions (= ParticleSets)
	//
	// The spaceship is previouly checked for collisions with asteroids on purpose as the colliding asteroid is just
	// marked, and any required actions will be performed after assuring that no race conditions will happen between
	// both threads. Therefore, the main thread can perform a little more work before being forced to wait for the
	// aux. one to finish its job.
	//
	// A similar approach could be applied to the following sections (check collisions -> mark colliding objects ->
	// sync both threads later on, therefore allowing them to achieve a greater parallel worload), but I believe that
	// the performance benefits of that would not outweight the code's greater complexity
	{
		// The main thread acquires exclusive access to the second section in which checks if the aux. one has
		// finished
		std::unique_lock<std::mutex> lock(mutexSection2);

		while (!auxThreadIterationFinished)
		{
			conditionSection2.wait(lock);
		}

	} // The lock is automatically released upon exiting this scope

	// A collision between the spaceship and an asteroid will also produce a tremor in the player's camera
	bool willGenerateTremor = false;

	if (spaceshipHasCollided)
	{
		collidingAsteroidWithSpaceship->explode();
		spaceship->explode();

		willGenerateTremor = true;
	}

	// Checking if any shot has collided with (destroyed) an asteroid
	for (unsigned int i = 0; i < asteroids.size(); i++)
	{
		for (unsigned int j = 0; j < shots.size(); j++)
		{
			if (shots.at(j)->checkCollision(asteroids.at(i)))
			{
				asteroids.at(i)->explode();

				// If a certain shot has destroyed an asteroid, the shots that are near it are also destroyed "due to
				// the expansion wave"
				std::vector<Shot *> shotsToRemove;

				for (unsigned int k = 0; k < shots.size(); k++)
				{
					// The specific maximum distance between the shot and those that are near enough to also be
					// removed is 100 units (the shot that destroys the asteroid will also be pushed into the vector)
					if (glm::distance(shots.at(k)->getPosition(), shots.at(j)->getPosition()) < 100.0f)
					{
						shotsToRemove.push_back(shots.at(k));
					}
				}

				while (shotsToRemove.size() > 0)
				{
					delete shotsToRemove.at(0); // A shot already removes itself in the set of shots present in the game scene
					shotsToRemove.erase(shotsToRemove.begin());
				}

				// The tremor effect can also be produced due to destroying an asteroid
				willGenerateTremor = true;

				// Counter fix as the current asteroid has been removed
				i--;

				// No need to check collisions between the already destroyed asteroid and the remaining shots
				break;
			}
		}
	}


	/* Final chores */

	// Storing time-related data that is needed to make decisions in future frames
	previousTime = currentTime;

	if (willGenerateTremor)
	{
		isShaking = true;
		lastShake = currentTime;
	}

	// If nothing has produced the tremor effect in the current effect, any previous enabling of the effect is
	// finished after 120 ms have elapsed
	else if (currentTime - lastShake > 0.12f)
	{
		isShaking = false;
	}
}

void notifyAuxThread()
{
	// The main thread acquires exclusive access to the first section in order to update the data through which the
	// aux. thread checks if it may proceed to update the particle's positions
	std::unique_lock<std::mutex> lock(mutexSection1);

	delta = currentTime - previousTime;
	newDelta = true;
	auxThreadIterationFinished = false;

	// The main thread also notifies the aux. one just in case it has already gone to sleep waiting for a new frame to
	// be rendered
	conditionSection1.notify_one();
}

void display()
{
	/* Drawing buffers are reset */

	// Color buffer is cleared by setting it to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// The depth buffer is also cleared
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	// All the available space in the window will be used for rendering
	glViewport(0, 0, wWidth, wHeight);

	// The interior of the polygons will be filled
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	/* All three transformation matrixes are computed */

	// Perspective projection
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(cameraFOV), (double)wWidth / (double)wHeight, 0.1,
		maxRenderDistance);

	// The Controller computes the view matrix as it depends on the user's input (the camera)
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;
	Controller::computeViewMatrix(viewMatrix, cameraPosition);

	// The model matrix is initialized to the identity matrix
	glm::mat4 modelMatrixBase = glm::mat4(1.0f);

	// A tremor is applied if the camera is currently being shaked
	if (isShaking)
	{
		modelMatrixBase = glm::translate(modelMatrixBase, glm::vec3(
			cosf(currentTime * 10.0f) * 2.0f,
			-sinf(currentTime * 10.0f) * 3.5f,
			-cosf(currentTime * 10.0f) * 2.7f
		));
	}


	/* Rendering using the main shader program (spaceship and asteroids) */

	// The two first matrixes are applied to the main shader program
	shader->use();
	shader->setMat4("projectionMatrix", projectionMatrix);
	shader->setMat4("viewMatrix", viewMatrix);

	// As well as the main light source and any additional source (from any existing spaceship's shot)
	mainLightSource->load(shader, glm::mat4(1.0f), 0);

	int lightCount = 1; // The main light has already been loaded

	// And the shader has a maximum allowed quantity of light sources
	int max = std::min(shots.size(), (size_t) (MAX_LIGHT_SOURCES - 1));
	for (int i = 0; i < max; i++)
	{
		shots.at(i)->getLight()->load(shader, glm::mat4(1.0f), lightCount);
		lightCount++;
	}

	// The shader needs to know how many light sources have been loaded, and where the camera is positioned
	shader->setInt("countLightSources", lightCount);
	shader->setVec3("cameraPosition", cameraPosition);

	if (!(spaceship->isDestroyed()))
	{
		spaceship->draw(modelMatrixBase, shader);
	}

	for (Asteroid *asteroid : asteroids)
	{
		asteroid->draw(modelMatrixBase, shader);
	}


	/* Rendering using the color shader program (shots) */

	shaderColor->use();
	shaderColor->setMat4("projectionMatrix", projectionMatrix);
	shaderColor->setMat4("viewMatrix", viewMatrix);

	for (Shot *shot : shots)
	{
		shot->draw(modelMatrixBase, shaderColor);
	}


	/* Rendering using the particle shader program (explosions + combustion effect) */

	shaderParticles->use();
	shaderParticles->setMat4("projectionMatrix", projectionMatrix);
	shaderParticles->setMat4("viewMatrix", viewMatrix);

	for (Particle *particle : particles)
	{
		particle->draw(modelMatrixBase, shaderParticles);
	}

	for (ParticleSet *particleSet : particleSets)
	{
		particleSet->draw(modelMatrixBase, shaderParticles);
	}


	/* Rendering using the skybox shader program */

	shaderSkybox->use();
	shaderSkybox->setMat4("projectionMatrix", projectionMatrix);
	shaderSkybox->setMat4("viewMatrix", glm::mat4(glm::mat3(viewMatrix)));

	skybox->draw(shaderSkybox);
}

void updateParticleSets(std::future<void> exitCondition)
{
	// The auxiliary thread will keep on running until the main thread commands it to stop in order to finish the
	// execution of the program
	while (true)
	{
		{
			// The aux. thread acquires exclusive access to the first section in order to check if it may proceed to
			// update the particle's positions
			std::unique_lock<std::mutex> lock(mutexSection1);

			while (!newDelta) // While no new frame is being rendered
			{
				conditionSection1.wait(lock);
			}

		} // The lock is automatically released upon exiting this scope

		// However, the main thread could just allow the aux. thread to proceed in order to let him know that it must
		// stop
		if (exitCondition.wait_for(std::chrono::microseconds(100)) != std::future_status::timeout)
		{
			break;
		}

		// Otherwise, the aux. thread proceeds to update the particles
		for (unsigned int i = 0; i < particleSets.size(); i++)
		{
			ParticleSet *particleSet = particleSets.at(i);

			particleSet->updateState(delta);

			// Removing dead particles
			if (particleSet->isDead())
			{
				delete particleSet;

				// The counter must be fixed in order to not skip any particles
				i--;
			}
		}

		{
			// The aux. thread acquires exclusive access to the second section in order to let the main thread know
			// that all particles' positions have been updated
			std::unique_lock<std::mutex> lock(mutexSection2);

			auxThreadIterationFinished = true;

			// The aux. thread also changes this variable in order to block itself the next time that it is ready to
			// update the particles; in other words, the aux. thread will wait until the main thread lets him know
			// that it must help in rendering a new frame
			newDelta = false;

			// It also notifies the main thread just in case the latter went to sleep in order to wait until the aux.
			// thread has updated all particles' positions, and thus the main thread can finish rendering the current
			// frame
			conditionSection2.notify_one();

		} // The lock is automatically released upon exiting this scope
	}
}
