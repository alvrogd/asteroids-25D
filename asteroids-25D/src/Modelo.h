#ifndef MODELO_H
#define MODELO_H

// Librerías de OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Para representarse
#include "Shader.h"

// Librerías de assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Se compondrá de meshes
#include "Mesh.h"

// Para conjuntos de datos
#include <vector>


class Modelo
{
public:

	/* Constructor */

	Modelo (const char *ruta);
	

	/* Getters y setters */

	std::vector<Mesh> & getMeshes ()
	{
		return this->meshes;
	}

	void setMeshes (std::vector<Mesh> meshes)
	{
		this->meshes = meshes;
	}

	std::string getDirectorio () const
	{
		return this->directorio;
	}

	void setDirectorio (std::string directorio)
	{
		this->directorio = directorio;
	}


	/* Métodos */

	void dibujar (glm::mat4 transformacionPadre, Shader *shader);


private:

	/* Atributos */

	// Información del modelo
	std::vector<Mesh> meshes;
	std::string directorio;


	/* Métodos */

	void cargarModelo (std::string ruta);

	void procesarNodo (aiNode *nodo, const aiScene *escena);

	Mesh procesarMesh (aiMesh *mesh, const aiScene *escena);

	std::vector<STextura> cargarTexturasMateriales (aiMaterial *material, aiTextureType tipo, std::string nombreTipo);
};

#endif
