#include "Cubemap.h"

float cubemapVertexes[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

Cubemap::Cubemap (std::vector<std::string> faces)
{
	// Needs its own VAO and VBO
	glGenVertexArrays (1, &(this->VAO));
	glGenBuffers (1, &(this->VBO));

	// The VAO is linked to configure it
	glBindVertexArray (this->VAO);

	// The VBO is linked
	glBindBuffer (GL_ARRAY_BUFFER, this->VBO);

	// All cubemap's vertex are copied to the VBO
	glBufferData (GL_ARRAY_BUFFER, sizeof(cubemapVertexes), cubemapVertexes, GL_STATIC_DRAW);

	// These properties are linked to the "input 0" in the shaders
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *)0);

	// Both the VAO and VBO are unlinked
	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	// The latter is no longer needed
	glDeleteBuffers (1, &(this->VBO));

	// The cubemap's textures must be loaded in OpenGL
	this->id = ImageReader::loadCubemap (faces);
}

void Cubemap::draw (Shader *shader) const
{
	// The depth-testing function must be altered as the cubemap has depth values of 1.0, and it would not be rendered
	// by default as the depth-testing is set to "valueDepthBuffer < 1.0"
	glDepthFunc (GL_LEQUAL);

	// The given shader is activated in order to render the cubemap
	shader->use ();

	glBindVertexArray (this->VAO);

	// The first texture unit is activated and the cubemap's textures are linked to it
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, this->id);

	// All the cubemap's triangles are rendered
	glDrawArrays (GL_TRIANGLES, 0, 36);

	glBindVertexArray (0);

	// The default depth-testing function is restored
	glDepthFunc (GL_LESS);
}
