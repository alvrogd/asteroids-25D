#include "Shape.h"


Sphere * Shape::sphere = NULL;


void Shape::initializeShapes ()
{
	Shape::sphere = new Sphere (1.0f, 6, 6);
}

void Shape::destroyShapes ()
{
	delete Shape::sphere;
}

void Shape::drawSphere ()
{
	Shape::sphere->draw ();
}
