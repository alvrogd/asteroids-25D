#include "Object.h"

Object::Object (glm::vec3 scaling, Model * model)
{
	this->scaling = scaling;
	this->model = model;
}

void Object::draw (glm::mat4 parentTransformation, Shader * shader)
{
	// The model that represents the object is commanded to render itself, using the given transformation matrix and
	// the object's scaling
	this->model->draw (glm::scale(parentTransformation, this->scaling), shader);
}
