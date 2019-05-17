#include "Modelo.h"

#include "LectorImagen.h"

Modelo::Modelo (const char * ruta)
{
	cargarModelo (ruta);
}

void Modelo::dibujar (glm::mat4 transformacionPadre, Shader *shader)
{
	// Se activa el shader dado
	shader->usar ();

	// Se inicializan las matrices a emplear
	glm::mat4 transformacion = transformacionPadre;
	glm::mat3 normalMatrix;

	// Se aplica la matriz de transformaciones al shader
	shader->setMat4 ("modelMatrix", transformacion);

	// Se calcula la matriz normal y se carga al shader
	normalMatrix = glm::transpose (glm::inverse (glm::mat3 (transformacion)));
	shader->setMat3 ("normalMatrix", normalMatrix);

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

	// Se cargan los datos del fichero dado, forzando además que triangule el modelo si no se encuentra formado
	// completamente por triángulos, y que invierta las coordenadas de textura en el eje Y (al ir estas en OpenGL al
	// "revés"
	const aiScene *escena = importador.ReadFile (ruta, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Se comprueba si se ha creado la escena correctamente, así como si se compone de al menos un nodo raíz
	if (!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importador.GetErrorString () << std::endl;
		return;
	}

	// Se guarda el directorio de donde se ha cargado el modelo
	this->directorio = ruta.substr(0, ruta.find_last_of('/'));

	// Y se procesa el nodo raíz cargado para crear el modelo
	procesarNodo (escena->mRootNode, escena);
}

void Modelo::procesarNodo (aiNode * nodo, const aiScene * escena)
{
	// Se procesan todos los meshes del nodo (cada nodo contiene índices de los meshes guardados en la escena de los que
	// se compone)
	for (unsigned int i = 0; i < nodo->mNumMeshes; i++)
	{
		aiMesh *mesh = escena->mMeshes[nodo->mMeshes[i]];
		this->meshes.push_back (procesarMesh (mesh, escena));
	}

	// Se indica a todos los nodos hijos que también se procesen
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

	// En primer lugar, se procesan todos los vértices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// Se almacenan en la estructura todas las propiedades del vértice iterado
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

		// Se almacena el vértice en el array de vértices
		vertices.push_back (vertice);
	}

	// Ahora, se cargan los índices de los vértices (cada cara es una primitiva, la cual se ha forzado que sea un
	// triángulo)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace cara = mesh->mFaces[i];

		for (unsigned int j = 0; j < cara.mNumIndices; j++)
		{
			indices.push_back (cara.mIndices[j]);
		}
	}

	// Finalmente, se carga el material (un mesh contiene un índice de un material de la escena)

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

std::vector<STextura> Modelo::cargarTexturasMateriales (aiMaterial *material, aiTextureType tipo, std::string nombreTipo)
{
	// Resultado
	std::vector<STextura> texturas;

	STextura textura;

	// Se recorren las texturas del tipo especificado en el material dado
	for (unsigned int i = 0; i < material->GetTextureCount (tipo); i++)
	{
		aiString localizacion;

		// Se obtiene la localización de la textura iterada
		material->GetTexture (tipo, i, &localizacion);

		// Se carga la información de la textura
		textura.id = LectorImagen::ficheroATextura (localizacion.C_Str ());

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
