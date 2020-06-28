#include "Model.h"

#include "../../utilities/ImageReader.h"

#include <iostream>


Model::Model (const char *path)
{
	loadModel (path);
}

void Model::draw (glm::mat4 parentTransformation, Shader *shader)
{
	shader->use ();

	glm::mat4 transformation = parentTransformation;
	glm::mat3 normalMatrix;

	shader->setMat4 ("modelMatrix", transformation);

	normalMatrix = glm::transpose (glm::inverse (glm::mat3 (transformation)));
	shader->setMat3 ("normalMatrix", normalMatrix);

	// All stored meshes are rendered
	for (unsigned int i = 0; i < this->meshes.size (); i++)
	{
		this->meshes.at (i).draw (shader);
	}
}

void Model::loadModel (std::string path)
{
	// An Assimp Importer will read the given file
	Assimp::Importer importer;

	// The importer is pointed to triangulate the model if it has shapes that are not triangles, and to invert the
	// coordianate textures in the Y axis (due to being "inverted" in OpenGL)
	const aiScene *scene = importer.ReadFile (path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// The resulting scene must also have a root node in order to be compatible
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString () << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/')) + "/";

	// The root node is processed in order to create the model
	processNode (scene->mRootNode, scene);
}

void Model::processNode (aiNode * node, const aiScene * scene)
{
	// Each node contains the indexes of the scene's meshes that it is made of
	// They are all processed and stored
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back (processMesh (mesh, scene));
	}

	// All children nodes are also processed, in order to fully process the resulting scene from the loaded model
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode (node->mChildren[i], scene);
	}
}

Mesh Model::processMesh (aiMesh * mesh, const aiScene * scene)
{
	std::vector<SVertex> vertexes;
	std::vector<unsigned int> indexes;
	std::vector<STexture> textures;

	SVertex vertex;

	// Firstly, all vertexes that make up the mesh are processed
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// The vertex's properties are retrieved
		vertex.position = glm::vec3 (mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		vertex.normal = glm::vec3 (mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		// The model may not have texture coordinates
		if (mesh->mTextureCoords[0])
		{
			vertex.textureCoordinates = glm::vec2 (mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		else
		{
			vertex.textureCoordinates = glm::vec2 (0.0f, 0.0f);
		}

		// The vertex is stored for latter usage
		vertexes.push_back (vertex);
	}

	// Secondly, the indexes of the vertexes are loaded
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indexes.push_back (face.mIndices[j]);
		}
	}

	// At last, the mesh's material is loaded, if any
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		// It should have both diffuse and specular maps
		std::vector<STexture> diffuseMaps = loadTexturesForMaterial (material, aiTextureType_DIFFUSE,
			"diffuseTexture");
		textures.insert (textures.end (), diffuseMaps.begin (), diffuseMaps.end ());

		std::vector<STexture> specularMaps = loadTexturesForMaterial (material, aiTextureType_SPECULAR,
			"specularTexture");
		textures.insert (textures.end (), specularMaps.begin (), specularMaps.end ());
	}

	// The resulting mesh is composed of all the retrieved data from the scene
	return Mesh (vertexes, indexes, textures);
}

std::vector<STexture> Model::loadTexturesForMaterial (aiMaterial *material, aiTextureType type,
	std::string typeName)
{
	std::vector<STexture> textures;

	STexture texture;

	// All textures of the specified type that are present in the given material
	for (unsigned int i = 0; i < material->GetTextureCount (type); i++)
	{
		aiString path;

		// Retrieving the location of the texture, and loading it
		material->GetTexture (type, i, &path);
		std::string texturePath = this->directory + path.C_Str();

		texture.id = ImageReader::textureFromFile (texturePath.c_str());
		texture.type = typeName;
		texture.path = path.C_Str ();

		textures.push_back (texture);
	}

	return textures;
}
