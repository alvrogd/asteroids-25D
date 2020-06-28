#include "Mesh.h"


#include <iostream>


Mesh::Mesh (std::vector<SVertex> vertexes, std::vector<unsigned int> indexes, std::vector<STexture> textures)
{
	this->vertexes = vertexes;
	this->indexes = indexes;
	this->textures = textures;

	// The mesh is configured using the given data
	configureMesh ();
}

void Mesh::draw (Shader * shader)
{
	// This method needs to know at anytime the index of the texture that is currently being processed, among those of
	// its type (the index starts at '1')
	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;

	// All the textures of the mesh are processed
	for (unsigned int i = 0; i < this->textures.size (); i++)
	{
		// One OpenGL texture unit for each texture of the mesh
		glActiveTexture (GL_TEXTURE0 + i);

		// It can be loaded using the ID that was generated when it was loaded
		glBindTexture (GL_TEXTURE_2D, this->textures.at(i).id);

		// The name of the 'uniform' type variable that corresponds the texture is generated at runtime
		std::string number;
		std::string name = this->textures.at (i).type;

		if (name == "diffuseTexture")
		{
			number = std::to_string (diffuseCount++);
		}

		else if (name == "specularTexture")
		{
			number = std::to_string (specularCount++);
		}

		else
		{
			std::cout << "ERROR::MESH::DRAW::TEXTURE_TYPE_UNKNOWN::" << name << std::endl;
		}

		// The texture unit number is loaded into the 'uniform' type variable
		shader->setInt (("material." + name + number).c_str (), i);
	}

	// And the mesh can now be rendered
	glBindVertexArray (this->VAO);
	glDrawElements (GL_TRIANGLES, this->indexes.size (), GL_UNSIGNED_INT, 0);
}

void Mesh::configureMesh ()
{
	// IDs of the buffers that the mesh needs in order to render itself in OpenGL
	glGenVertexArrays (1, &(this->VAO));
	glGenBuffers (1, &(this->VBO));
	glGenBuffers (1, &(this->EBO));

	// The VAO is linked to configure it
	glBindVertexArray (this->VAO);

	// Firstly, the VBO is configured along the VAO
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// It holds the data of all the given vertexes
	glBufferData (GL_ARRAY_BUFFER, this->vertexes.size () * sizeof (SVertex), this->vertexes.data (),
		GL_STATIC_DRAW);

	// Secondly, the EBO is configured
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->EBO);

	// It holds the data of all the given indexes
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, this->indexes.size () * sizeof (unsigned int), this->indexes.data (),
		GL_STATIC_DRAW);


	// The loaded info needs to be configured so that shaders are able to read it

	// Vertexes' positions
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (SVertex), (void *)0);

	// Vertexes' normals
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (SVertex), (void *)offsetof (SVertex, normal));

	// Vertexes' texture coordinates
	glEnableVertexAttribArray (2);
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof (SVertex), (void *)offsetof (SVertex,
		textureCoordinates));

	// VAO does not longer need to be linked
	glBindVertexArray (0);

	// However, the buffers can be erased as their data has already been linked to the VAO
	glDeleteBuffers (1, &(this->VBO));
	glDeleteBuffers (1, &(this->EBO));
}
