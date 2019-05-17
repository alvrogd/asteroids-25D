#ifndef FORMA_H
#define FORMA_H

// Para representar esferas
#include "Esfera.h"


class Forma
{
public:

	/* Getters y setters */

	static Esfera & getEsfera ()
	{
		return *(Forma::esfera);
	}

	static void setEsfera (Esfera &esfera)
	{
		*(Forma::esfera) = esfera;
	}


	/* Métodos */

	static void inicializarFormas ();

	static void destruirFormas ();

	static void dibujarEsfera ();


private:

	/* Atributos */

	static Esfera *esfera;
};

#endif
