#include "Material.h"

Material::Material (glm::vec3 ambiental, glm::vec3 difusa, glm::vec3 especular, float brillo)
{
	// Se guardan todos los valores dados
	this->ambiental = ambiental;
	this->difusa = difusa;
	this->especular = especular;

	this->brillo = brillo;
}

void Material::cargar (Shader * shader) const
{
	// Se cargan en el fragment shader las propiedades del material
	shader->setVec3 ("material.ambiental", this->ambiental);
	shader->setVec3 ("material.difusa", this->difusa);
	shader->setVec3 ("material.especular", this->especular);

	shader->setFloat ("material.brillo", this->brillo);
}
