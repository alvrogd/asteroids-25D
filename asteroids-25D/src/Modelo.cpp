#include "Modelo.h"

// Carga de im�genes
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
// Para cargar im�genes del disco
#include <stb_image.h>
#endif

Modelo::Modelo (const char * ruta)
{
	cargarModelo (ruta);
}

void Modelo::dibujar (Shader * shader)
{
	// Se representan todos los meshes guardados
	for (unsigned int i = 0; i < this->meshes.size (); i++)
	{
		this->meshes.at (i).dibujar (shader);
	}
}

void Modelo::cargarModelo (std::string ruta)
{
	// Se crea un importer de Assimp para que se encargue de leer el fichero dado
	Assimp::Importer importador;

	// Se cargan los datos del fichero dado, forzando adem�s que triangule el modelo si no se encuentra formado
	// completamente por tri�ngulos, y que invierta las coordenadas de textura en el eje Y (al ir estas en OpenGL al
	// "rev�s"
	const aiScene *escena = importador.ReadFile (ruta, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Se comprueba si se ha creado la escena correctamente, as� como si se compone de al menos un nodo ra�z
	if (!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importador.GetErrorString () << std::endl;
		return;
	}

	// Se guarda el directorio de donde se ha cargado el modelo
	this->directorio = ruta.substr(0, ruta.find_last_of('/'));

	// Y se procesa el nodo ra�z cargado para crear el modelo
	procesarNodo (escena->mRootNode, escena);
}

void Modelo::procesarNodo (aiNode * nodo, const aiScene * escena)
{
	// Se procesan todos los meshes del nodo (cada nodo contiene �ndices de los meshes guardados en la escena de los que
	// se compone)
	for (unsigned int i = 0; i < nodo->mNumMeshes; i++)
	{
		aiMesh *mesh = escena->mMeshes[nodo->mMeshes[i]];
		this->meshes.push_back (procesarMesh (mesh, escena));
	}

	// Se indica a todos los nodos hijos que tambi�n se procesen
	for (unsigned int i = 0; i < nodo->mNumChildren; i++)
	{
		procesarNodo (nodo->mChildren[i], escena);
	}
}

Mesh Modelo::procesarMesh (aiMesh * mesh, const aiScene * escena)
{
	// Contenidos del mesh resultado
	std::vector<SVertice> vertices;
	std::vector<unsigned int> indices;
	std::vector<STextura> texturas;

	SVertice vertice;

	// En primer lugar, se procesan todos los v�rtices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// Se almacenan en la estructura todas las propiedades del v�rtice iterado
		vertice.posicion = glm::vec3 (mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		vertice.normal = glm::vec3 (mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		// A lo mejor el modelo no contiene coordenadas de texturas
		if (mesh->mTextureCoords[0])
		{
			vertice.coordenadasTexturas = glm::vec2 (mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		else
		{
			vertice.coordenadasTexturas = glm::vec2 (0.0f, 0.0f);
		}

		// Se almacena el v�rtice en el array de v�rtices
		vertices.push_back (vertice);
	}

	// Ahora, se cargan los �ndices de los v�rtices (cada cara es una primitiva, la cual se ha forzado que sea un
	// tri�ngulo)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace cara = mesh->mFaces[i];

		for (unsigned int j = 0; j < cara.mNumIndices; j++)
		{
			indices.push_back (cara.mIndices[j]);
		}
	}

	// Finalmente, se carga el material (un mesh contiene un �ndice de un material de la escena)

	// Si contiene un materiale
	if (mesh->mMaterialIndex >= 0)
	{
		// Se carga
		aiMaterial *material = escena->mMaterials[mesh->mMaterialIndex];

		// Se cargan sus mapas difusos y se almacenan en el vector de texturas
		std::vector<STextura> mapasDifusos = cargarTexturasMateriales (material, aiTextureType_DIFFUSE, "texturaDifusa");
		texturas.insert (texturas.end (), mapasDifusos.begin (), mapasDifusos.end ());

		// Se cargan sus mapas especulares y se almacenan en el vector de texturas
		std::vector<STextura> mapasEspeculares = cargarTexturasMateriales (material, aiTextureType_SPECULAR, "texturaEspecular");
		texturas.insert (texturas.end (), mapasEspeculares.begin (), mapasEspeculares.end ());
	}

	// Se devuelve un mesh con los datos obtenidos
	return Mesh (vertices, indices, texturas);
}

std::vector<STextura> Modelo::cargarTexturasMateriales (aiMaterial * material, aiTextureType tipo, std::string nombreTipo)
{
	// Resultado
	std::vector<STextura> texturas;

	STextura textura;

	// Se recorren las texturas del tipo especificado en el material dado
	for (unsigned int i = 0; i < material->GetTextureCount (tipo); i++)
	{
		aiString localizacion;

		// Se obtiene la localizaci�n de la textura iterada
		material->GetTexture (tipo, i, &localizacion);

		// Se carga la informaci�n de la textura
		textura.id = ficheroATextura (localizacion.C_Str (), directorio);

		// Se almacena el tipo dado
		textura.tipo = nombreTipo;

		// Se almacena la ruta obtenida
		textura.ruta = localizacion.C_Str ();

		// Y se almacena la textura en el vector de texturas
		texturas.push_back (textura);
	}

	// Se devuelven las texturas cargadas
	return texturas;
}

unsigned int Modelo::ficheroATextura (const char * path, const std::string & directory)
{

	std::string filename = std::string (path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures (1, &textureID);

	int width, height, nrComponents;
	//unsigned char *data = stbi_load (filename.c_str (), &width, &height, &nrComponents, 0);
	unsigned char *data = stbi_load (path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture (GL_TEXTURE_2D, textureID);
		glTexImage2D (GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap (GL_TEXTURE_2D);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free (data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		std::cout << "Texture failed to load at filename: " << filename.c_str () << std::endl;
		stbi_image_free (data);
	}

	return textureID;
}
