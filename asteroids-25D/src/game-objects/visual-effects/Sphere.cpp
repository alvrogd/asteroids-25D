#include "Sphere.h"

Sphere::Sphere (float radius, int sectors, int stacks)
{
	this->radius = radius;
	this->sectors = sectors;
	this->stacks = stacks;

	buildSphere ();
}

Sphere::~Sphere ()
{
	glDeleteVertexArrays (1, &(this->VAO));
}

void Sphere::buildSphere ()
{
	// http://www.songho.ca/opengl/gl_sphere.html

	generateVertexes ();

	generateIndexes ();
	
	blendVertexesWithProperties ();

	generateVAO ();
}

void Sphere::generateVertexes ()
{
	// Frees memory just in case the sphere is being built again
	std::vector<float> ().swap (vertexes);
	std::vector<float> ().swap (normals);
	std::vector<float> ().swap (textureCoordinates);

	// Current vertex's positions
	float x, y, z, xy;
	// Current vertex's normals
	float nX, nY, nZ, inverseLongitude = 1.0f / radius;
	// Current vertex's texture coordinates
	float s, t;

	// Increment between...
	float incrementSectors = 2 * (float)M_PI / sectors;
	// Paso entre stacks
	float incrementStacks = (float)M_PI / stacks;

	// Current vertex's angles
	float angleSector, angleStack;

	int i, j;


	// Going along the sphere's latitude
	for (i = 0; i <= stacks; i++)
	{
		// From PI/2 (top) to -PI/2 (bottom)
		angleStack = (float)M_PI / 2 - i * incrementStacks;

		// Auxiliar value to compute the X and Y coordinates (vector that lies on the XYO plane); r * cos(u) 
		xy = radius * cosf (angleStack);

		// Z coordinate; r * sin(u)
		z = radius * sinf (angleStack);

		// Each stack gets "sectors + 1" vertexes; the first and last vertexes have both the same position and normal,
		// but different texture coordinates
		for (j = 0; j <= sectors; j++)
		{
			// From 0 to 2*PI
			angleSector = j * incrementSectors;

			// Vertex's X and Y coordinates
			x = xy * cosf (angleSector);
			y = xy * sinf (angleSector);

			// Storing the vertex's coordinates
			vertexes.push_back (x);
			vertexes.push_back (y);
			vertexes.push_back (z);

			// Computing and storing normals (sphere -> they correspond to the coordinates), which also get normalized
			// (multiplying by the inverse longitude)
			nX = x * inverseLongitude;
			nY = y * inverseLongitude;
			nZ = z * inverseLongitude;

			normals.push_back (nX);
			normals.push_back (nY);
			normals.push_back (nZ);

			// Computing and storing the texture coordinates, which fall in the interval [0, 1]
			s = (float)j / sectors;
			t = (float)i / stacks;

			textureCoordinates.push_back (s);
			textureCoordinates.push_back (t);
		}
	}
}

void Sphere::generateIndexes ()
{
	int k1, k2;

	int i, j;


	// Going along the sphere's latitude
	for (i = 0; i < stacks; i++)
	{
		// Initial vertex (top) of the current stack
		k1 = i * (sectors + 1);
		// Initial vertex of the next stack (current stack's bottom vertex)
		k2 = k1 + sectors + 1;

		// Going along the sphere's longitude
		for (j = 0; j < sectors; j++, k1++, k2++)
		{
			// Each sector is made up of two triangles
			//  - The first and last sector are exceptions to this rule

			// Not the first stack
			if (i != 0)
			{
				// Triángulo superior/izquierdo
				vertexIndexes.push_back (k1);
				vertexIndexes.push_back (k2);
				vertexIndexes.push_back (k1 + 1);
			}

			// Not the last stack
			if (i != stacks - 1)
			{
				vertexIndexes.push_back (k1 + 1);
				vertexIndexes.push_back (k2);
				vertexIndexes.push_back (k2 + 1);
			}
		}
	}
}

void Sphere::blendVertexesWithProperties ()
{
	std::size_t vertexCount = vertexes.size ();
	std::size_t i, j;


	std::vector<float> ().swap (interleavedVertexes);

	// The info of each vertex is pushed into the vector
	for (i = 0, j = 0; i < vertexCount; i += 3, j += 2)
	{
		// Its coordinates
		interleavedVertexes.push_back (vertexes[i]);
		interleavedVertexes.push_back (vertexes[i + 1]);
		interleavedVertexes.push_back (vertexes[i + 2]);

		// Its normals
		interleavedVertexes.push_back (normals[i]);
		interleavedVertexes.push_back (normals[i + 1]);
		interleavedVertexes.push_back (normals[i + 2]);

		// Its texture coordinates
		interleavedVertexes.push_back (textureCoordinates[j]);
		interleavedVertexes.push_back (textureCoordinates[j + 1]);
	}
}

void Sphere::generateVAO ()
{
	// Just in case the sphere is being rebuilt
	glDeleteVertexArrays (1, &VAO);

	glGenVertexArrays (1, &VAO);
	glGenBuffers (1, &VBO);
	glGenBuffers (1, &EBO);

	// Linking the VAO to configure it
	glBindVertexArray (VAO);

	// The VBO goes first
	glBindBuffer (GL_ARRAY_BUFFER, VBO);

	// Each vertex's properties are copied to the buffer
	glBufferData (GL_ARRAY_BUFFER, interleavedVertexes.size () * sizeof (float), &interleavedVertexes[0],
		GL_STATIC_DRAW);

	// Now goes the EBO
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Whose data will be the contents of the indexes vector
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, vertexIndexes.size () * sizeof (int), &vertexIndexes[0], GL_STATIC_DRAW);

	// Specifying how each vertex's coordinates are layed out in memory, as well as linking them to the "0" attribute
	// in shaders
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)0);
	glEnableVertexAttribArray (0);

	// Specifying how each vertex's colors are layed out in memory, as well as linking them to the "1" attribute
	// in shaders
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(3 * sizeof (float)));
	glEnableVertexAttribArray (1);

	// Specifying how each vertex's texture coordinates are layed out in memory, as well as linking them to the
	// "2" attribute in shaders
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(6 * sizeof (float)));
	glEnableVertexAttribArray (2);

	// The VAO gets unlinked as its configuration has finished
	glBindVertexArray (0);

	// And the buffers are no longer needed as their contents are bound to the VAO
	glDeleteBuffers (1, &VBO);
	glDeleteBuffers (1, &EBO);
}

void Sphere::draw () const
{
	glBindVertexArray (VAO);

	// Rendering the triangles that make up the sphere
	glDrawElements (GL_TRIANGLES, vertexIndexes.size (), GL_UNSIGNED_INT, 0);

	glBindVertexArray (0);
}
