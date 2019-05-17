#version 330 core

/* Estructuras para información sobre los objetos de la escena */

struct Material {
	// Reflexión de los componentes RGB para cada uno de los componentes de la luz
	sampler2D texturaDifusa1;
	sampler2D texturaEspecular1;

	// Brillo que determina la concentración con la luz especular
	float brillo;
};

struct PuntoLuz {
	// Posición del punto de luz
	vec3 posicion;

	// Atenuación de la luz en función de la distancia
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

// Variables globales necesarias para la composición de los fragmentos
uniform vec3 posicionCamara;
uniform Material material;

// Putos de luz presentes en la escena
#define N_PUNTOS_LUZ 50
uniform int numPuntosLuz;
uniform PuntoLuz puntosLuz[N_PUNTOS_LUZ];


// Cálculo de la contribuición de un punto de luz a la escena
vec3 calcularPuntoLuz(PuntoLuz puntoLuz, vec3 normal, vec3 fragPos, vec3 direccionCamara)
{
	// Es necesaria la dirección entre la luz y el fragmento; se obtiene en el sentido luz-fragmento
	vec3 direccionLuz = normalize(puntoLuz.posicion - fragPos);

	// Se calcula el ángulo entre la normal y la dirección de indicendia de la luz, el cual variará la intensidad; se
	// evita que el valor baje de '0' porque el comportamiento no está definido para los colores
	float diferencia = max(dot(normal, direccionLuz), 0.0);


	// Se obtiene la componente especualar en función de la luz difusa del punto de luz y de la propiedad difusa del
	// especular

	// Ahora se calcula el vector de reflexión de la luz indicente; se niega la dirección de la luz porque espera que
	// esté en el sentido contrario
	vec3 direccionReflexion = reflect(-direccionLuz, normal);

	// Se calcula el ángulo entre la dirección de reflexión y la dirección de la cámara, el cual variará la intensidad;
	// se evita también que el valor baje de '0', y se tiene además en cuenta el grado de brillo del material
	float spec = pow(max(dot(direccionCamara, direccionReflexion), 0.0), 1.0f);
	
	
	// Se aplica a los cálculos hechos la atenuación correspondiente en función de la distancia entre la fuente de luz y
	// el fragmento
	float distancia = length(puntoLuz.posicion - fragPos);
	float atenuacion = 1.0 / (puntoLuz.constante + puntoLuz.lineal * distancia + puntoLuz.cuadratica * (distancia * distancia));


	// Se combinan los resultados obtenidos
	vec3 ambiental = puntoLuz.ambiental * vec3(texture(material.texturaDifusa1, TexCoord));
	vec3 difusa = puntoLuz.difusa * diferencia * vec3(texture(material.texturaDifusa1, TexCoord));
	vec3 especular = puntoLuz.especular * spec * vec3(texture(material.texturaEspecular1, TexCoord));

	// Se les aplica la atenuación calculada
	ambiental *= atenuacion;
	difusa *= atenuacion;
	especular *= atenuacion;

	// Se devuelve la suma de las componentes calculadas
	return(ambiental + difusa + especular);
}

void main()
{
	// Para el cálculo de las componentes difusa y especular es necesaria la normal del fragmento; se normaliza por si
	// acaso
	vec3 norm = normalize(Normal);

	// Para el cálculo de la componente especular es necesaria la dirección en la que la cámara mira al fragmento a
	// renderizar
	vec3 direccionCamara = normalize(posicionCamara - FragPos);

	// Se calculan las aportaciones a la iluminación de los puntos de luz especificados
	vec3 resultado = vec3(0.0f, 0.0f, 0.0f);
	for(int i = 0; i < numPuntosLuz; i++)
	{
		resultado += calcularPuntoLuz(puntosLuz[i], norm, FragPos, direccionCamara);
	}
	
	FragColor = vec4(resultado, 1.0f);
}