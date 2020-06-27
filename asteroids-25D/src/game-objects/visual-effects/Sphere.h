#ifndef SPHERE_H
#define SPHERE_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Sphere
{
public:

	/* Constructor */

	Sphere (float radius, int sectors, int stacks);


	/* Destructor */

	~Sphere ();


	/* Methods */

	void draw() const;


private:

	/* Attributes */

	float radius;

	int sectors; // Longitude

	int stacks; // Latitude

	std::vector<float> vertexes;

	std::vector<float> normals;

	std::vector<float> textureCoordinates;

	std::vector<float> interleavedVertexes; // Vertexes + their properties

	std::vector<int> vertexIndexes; // They determine how the sphere is rendered

	unsigned int VBO = 0, EBO = 0, VAO = 0;


	/* Methods */

	void buildSphere ();

	void generateVertexes ();

	// Generates the indexes of the triangles that make up the sphere, using the previously generated vertexes
	void generateIndexes ();

	// Bleding results in a single vector
	void blendVertexesWithProperties ();

	// The sphere will be rendered through its VAO
	void generateVAO ();
};

#endif
