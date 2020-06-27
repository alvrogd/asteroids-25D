#ifndef MODEL_H
#define MODEL_H

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// To render itself
#include "../../utilities/Shader.h"

// Assimp libraries
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// 1 Model -- 1..* Meshes
#include "Mesh.h"

#include <vector>


class Model
{
public:

	/* Constructor */

	Model (const char *path);
	

	/* Getters & setters */

	std::vector<Mesh> & getMeshes ()
	{
		return this->meshes;
	}

	void setMeshes (std::vector<Mesh> meshes)
	{
		this->meshes = meshes;
	}

	std::string getDirectory () const
	{
		return this->directory;
	}

	void setDirectory (std::string directory)
	{
		this->directory = directory;
	}


	/* Methods */

	void draw (glm::mat4 parentTransformation, Shader *shader);


private:

	/* Attributes */

	std::vector<Mesh> meshes;
	std::string directory;


	/* Methods */

	void loadModel (std::string path);

	void processNode (aiNode *node, const aiScene *scene);

	Mesh processMesh (aiMesh *mesh, const aiScene *scene);

	std::vector<STexture> loadTexturesForMaterial (aiMaterial *material, aiTextureType type, std::string typeName);
};

#endif
