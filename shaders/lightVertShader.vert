#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightCol;
uniform vec3 objectCol;

out vec3 GouraudColor;

void main()
{
	vec3 LightPosViewSpace = vec3(view * vec4(lightPos, 1.0));
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
	vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
	
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
	GouraudColor = (ambientLight + diffuseLight + specularLight) * objectCol;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}