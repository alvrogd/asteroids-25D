#include "LightSource.h"

LightSource::LightSource (glm::vec3 position, float constant, float lineal, float quadratic, glm::vec3 ambient,
	glm::vec3 diffuse, glm::vec3 specular)
{
	this->position = position;

	this->constant = constant;
	this->lineal = lineal;
	this->quadratic = quadratic;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void LightSource::load (Shader * shader, glm::mat4 transformation, int lightNumber) const
{
	// The light source's properties are loaded into the fragment shader

	// The given transformation is also applied to its position
	glm::vec4 transformedPosition = transformation * glm::vec4 (this->position, 1.0f);
	shader->setVec3 ("lightSources[" + std::to_string(lightNumber) + "].position", glm::vec3(transformedPosition));

	shader->setFloat ("lightSources[" + std::to_string (lightNumber) + "].constant", this->constant);
	shader->setFloat ("lightSources[" + std::to_string (lightNumber) + "].lineal", this->lineal);
	shader->setFloat ("lightSources[" + std::to_string (lightNumber) + "].quadratic", this->quadratic);

	shader->setVec3 ("lightSources[" + std::to_string (lightNumber) + "].ambient", this->ambient);
	shader->setVec3 ("lightSources[" + std::to_string (lightNumber) + "].diffuse", this->diffuse);
	shader->setVec3 ("lightSources[" + std::to_string (lightNumber) + "].specular", this->specular);
}
