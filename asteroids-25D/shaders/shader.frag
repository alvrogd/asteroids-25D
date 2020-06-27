#version 330 core

/* Structs that hold data of the currently shown objects in the scene */

struct Material {

	// Reflection of each RGB component for both diffuse and specular light
	sampler2D diffuseTexture1;
	sampler2D specularTexture1;

	// Determines the concentration of specular light
	float brightness;
};

struct LightSource {

	vec3 position;

	// Attenuation rate, depends on the distance between the light and the camera
	float constant;
	float lineal;
	float quadratic;

	// RGB components for each one of the light source's components
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


// Input
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Output
out vec4 FragColor;

// Global variables set directly from the CPU
uniform vec3 cameraPosition;
uniform Material material;

// Light sources present in the scene
#define MAX_LIGHT_SOURCES 50
uniform int countLightSources;
uniform LightSource lightSources[MAX_LIGHT_SOURCES];


// Computes the contribution of a certain light source
vec3 computeLightSource(LightSource lightSource, vec3 normal, vec3 fragPos, vec3 cameraDirection)
{
	// Direction from the light source to the current fragment that is being rendered
	vec3 lightDirection = normalize(lightSource.position - fragPos);

	// Computing the angle between the normal and the light source's incidence direction, which will determine the
	// intensity; its value cannot be less than '0' as it would be undefined behaviour
	float deviationAngle = max(dot(normal, lightDirection), 0.0);


	// The shader will now compute data for the specular component

	// Computing the reflection vector of the incident light; the direction needs to be inverted according to how it
	// was previously computed
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	// Computing the angle between the reflection direction and the camera's direction, as it will determine the
	// intensity; its value cannot be less than '0' as it would be undefined behaviour
	//	- Usually, the material's brightness would be used here instead of a raw value
	float spec = pow(max(dot(cameraDirection, reflectionDirection), 0.0), 1.0f);
	
	
	// The light source's contribution will also decrease as its distance to the fragment increases
	float cDistance = length(lightSource.position - fragPos);
	float attenuation = 1.0 / (lightSource.constant + lightSource.lineal * cDistance + lightSource.quadratic * (cDistance * cDistance));


	// The light source's contributions also need to be fused with the properties of the fragment's material
	vec3 ambient = lightSource.ambient * vec3(texture(material.diffuseTexture1, TexCoord));
	vec3 diffuse = lightSource.diffuse * deviationAngle * vec3(texture(material.diffuseTexture1, TexCoord));
	vec3 specular = lightSource.specular * spec * vec3(texture(material.specularTexture1, TexCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return(ambient + diffuse + specular);
}

void main()
{
	// The fragment's normal is needed in order to compute the diffuse and specular components of the light source
	// It is convenient to normalize it (better safe than sorry)
	vec3 norm = normalize(Normal);

	// The direction in which the camera looks at the current fragment is also needed in order to compute the specular component
	vec3 cameraDirection = normalize(cameraPosition - FragPos);

	// The contribution of all present light sources is computed
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < countLightSources; i++)
	{
		result += computeLightSource(lightSources[i], norm, FragPos, cameraDirection);
	}
	
	FragColor = vec4(result, 1.0f);
}