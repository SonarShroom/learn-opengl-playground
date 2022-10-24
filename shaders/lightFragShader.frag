#version 330 core

in vec3 LightPosViewSpace;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectCol;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 viewPos;

void main()
{
	// Ambient component
	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * lightCol;
	
	// Diffuse component
	vec3 _normal = normalize(Normal);
	vec3 lightDirection = normalize(LightPosViewSpace - FragPos);

	float diff = max(dot(_normal, lightDirection), 0.0);
	vec3 diffuseLight = diff * lightCol;
	
	// Specular component
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(-FragPos);
	vec3 reflectDirection = reflect(-lightDirection, _normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specularLight = specularStrength * spec * lightCol;

	// Result
	vec3 result = (ambientLight + diffuseLight + specularLight) * objectCol;
	FragColor = vec4(result, 1.0);
}