#ifndef SHAPE_H
#define SHAPE_H

// To render spheres
#include "Sphere.h"


class Shape
{
public:

	/* Getters & setters */

	static Sphere & getSphere ()
	{
		return *(Shape::sphere);
	}

	static void setSphere (Sphere &sphere)
	{
		*(Shape::sphere) = sphere;
	}


	/* Methods */

	static void initializeShapes ();

	static void destroyShapes ();

	static void drawSphere ();


private:

	/* Attributes */

	static Sphere *sphere;
};

#endif
