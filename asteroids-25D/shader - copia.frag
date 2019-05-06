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
uniform PuntoLuz puntoLuz;

void main()
{
	// Se obtiene la componente ambiental en función de la luz ambiental del punto de luz y de la propiedad ambiental del
	// material
	vec3 ambiental = puntoLuz.ambiental * vec3(texture(material.texturaDifusa1, TexCoord));

	
	// Se obtiene la componente difusa en función de la luz difusa del punto de luz y de la propiedad difusa del material

	// Para el cálculo es necesaria la normal del fragmento; se normaliza por si acaso
	vec3 norm = normalize(Normal);

	// Es necesaria la dirección entre la luz y el fragmento; se obtiene en el sentido luz-fragmento
	vec3 direccionLuz = normalize(puntoLuz.posicion - FragPos);

	// Se calcula el ángulo entre la normal y la dirección de indicendia de la luz, el cual variará la intensidad; se
	// evita que el valor baje de '0' porque el comportamiento no está definido para los colores
	float diferencia = max(dot(norm, direccionLuz), 0.0);
	
	// Ahora se puede calcular ya el resultado de la componente difusa
	vec3 difusa = puntoLuz.difusa * diferencia * vec3(texture(material.texturaDifusa1, TexCoord));


	// Se obtiene la componente especualar en función de la luz difusa del punto de luz y de la propiedad difusa del
	// especular

	// Es necesaria la dirección entre la cámara y el fragmento; se obtiene en el sentido cámara-fragmento
	vec3 direccionCamara = normalize(posicionCamara - FragPos);

	// Ahora se calcula el vector de reflexión de la luz indicente; se niega la dirección de la luz porque espera que
	// esté en el sentido contrario
	vec3 direccionReflexion = reflect(-direccionLuz, norm);

	// Se calcula el ángulo entre la dirección de reflexión y la dirección de la cámara, el cual variará la intensidad;
	// se evita también que el valor baje de '0', y se tiene además en cuenta el grado de brillo del material
	float spec = pow(max(dot(direccionCamara, direccionReflexion), 0.0), 1.0f);
	
	// Ahora se puede calcular ya el resultado de la componente especular
	vec3 especular = puntoLuz.especular * spec * vec3(texture(material.texturaEspecular1, TexCoord));
	
	
	// Se aplica a los cálculos hechos la atenuación correspondiente en función de la distancia entre la fuente de luz y
	// el fragmento
	float distancia = length(puntoLuz.posicion - FragPos);
	float atenuacion = 1.0 / (puntoLuz.constante + puntoLuz.lineal * distancia + puntoLuz.cuadratica * (distancia * distancia));

	ambiental *= atenuacion;
	difusa *= atenuacion;
	especular *= atenuacion;

	
	// El color resultado es la suma de las tres componentes multiplicados por los contenido de la texturas cargadas en
	// las coordenadas dadas (80% de la 1ª y 20% de la 2ª mediante interpolado lineal)
	vec3 resultado = ambiental + difusa + especular;
	
	FragColor = vec4(resultado, 1.0f);
	//FragColor = vec4(vec3(texture(material.texturaDifusa1, TexCoord)), 1.0f);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}