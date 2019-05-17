#version 330 core

/* Estructuras para informaci�n sobre los objetos de la escena */

struct Material {
	// Reflexi�n de los componentes RGB para cada uno de los componentes de la luz
	sampler2D texturaDifusa1;
	sampler2D texturaEspecular1;

	// Brillo que determina la concentraci�n con la luz especular
	float brillo;
};

struct PuntoLuz {
	// Posici�n del punto de luz
	vec3 posicion;

	// Atenuaci�n de la luz en funci�n de la distancia
	float constante;
	float lineal;
	float cuadratica;

	// Componentes RGB para cada una de las componentes del punto de luz
	vec3 ambiental;
	vec3 difusa;
	vec3 especular;
};


// Input
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Salida del shader
out vec4 FragColor;

// Variables globales necesarias para la composici�n de los fragmentos
uniform vec3 posicionCamara;
uniform Material material;

// Putos de luz presentes en la escena
#define N_PUNTOS_LUZ 50
uniform int numPuntosLuz;
uniform PuntoLuz puntosLuz[N_PUNTOS_LUZ];


// C�lculo de la contribuici�n de un punto de luz a la escena
vec3 calcularPuntoLuz(PuntoLuz puntoLuz, vec3 normal, vec3 fragPos, vec3 direccionCamara)
{
	// Es necesaria la direcci�n entre la luz y el fragmento; se obtiene en el sentido luz-fragmento
	vec3 direccionLuz = normalize(puntoLuz.posicion - fragPos);

	// Se calcula el �ngulo entre la normal y la direcci�n de indicendia de la luz, el cual variar� la intensidad; se
	// evita que el valor baje de '0' porque el comportamiento no est� definido para los colores
	float diferencia = max(dot(normal, direccionLuz), 0.0);


	// Se obtiene la componente especualar en funci�n de la luz difusa del punto de luz y de la propiedad difusa del
	// especular

	// Ahora se calcula el vector de reflexi�n de la luz indicente; se niega la direcci�n de la luz porque espera que
	// est� en el sentido contrario
	vec3 direccionReflexion = reflect(-direccionLuz, normal);

	// Se calcula el �ngulo entre la direcci�n de reflexi�n y la direcci�n de la c�mara, el cual variar� la intensidad;
	// se evita tambi�n que el valor baje de '0', y se tiene adem�s en cuenta el grado de brillo del material
	float spec = pow(max(dot(direccionCamara, direccionReflexion), 0.0), 1.0f);
	
	
	// Se aplica a los c�lculos hechos la atenuaci�n correspondiente en funci�n de la distancia entre la fuente de luz y
	// el fragmento
	float distancia = length(puntoLuz.posicion - fragPos);
	float atenuacion = 1.0 / (puntoLuz.constante + puntoLuz.lineal * distancia + puntoLuz.cuadratica * (distancia * distancia));


	// Se combinan los resultados obtenidos
	vec3 ambiental = puntoLuz.ambiental * vec3(texture(material.texturaDifusa1, TexCoord));
	vec3 difusa = puntoLuz.difusa * diferencia * vec3(texture(material.texturaDifusa1, TexCoord));
	vec3 especular = puntoLuz.especular * spec * vec3(texture(material.texturaEspecular1, TexCoord));

	// Se les aplica la atenuaci�n calculada
	ambiental *= atenuacion;
	difusa *= atenuacion;
	especular *= atenuacion;

	// Se devuelve la suma de las componentes calculadas
	return(ambiental + difusa + especular);
}

void main()
{
	// Para el c�lculo de las componentes difusa y especular es necesaria la normal del fragmento; se normaliza por si
	// acaso
	vec3 norm = normalize(Normal);

	// Para el c�lculo de la componente especular es necesaria la direcci�n en la que la c�mara mira al fragmento a
	// renderizar
	vec3 direccionCamara = normalize(posicionCamara - FragPos);

	// Se calculan las aportaciones a la iluminaci�n de los puntos de luz especificados
	vec3 resultado = vec3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < numPuntosLuz; i++)
	{
		resultado += calcularPuntoLuz(puntosLuz[i], norm, FragPos, direccionCamara);
	}
	
	FragColor = vec4(resultado, 1.0f);
}