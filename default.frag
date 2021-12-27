#version 330 core
out vec4 FragColor;
in vec3 currPos; // the input variable from the vertex shader (same name and same type)
in vec3 Normal;
in vec3 color; 
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight(vec3 normal, vec3 lightVec, float lightDis, vec3 lightDir, float ambient, float diffuse, float specularIntensity, int specPow) {
	// This light dims after a certain extent, which is modulated using intensity with the inverse square law.
	// Lights objects in all directions equally.

	// Light Eqn, inverse square law
	float a = 0.2;
	float b = 0.05;
	float intensity = 1.0f / (a * lightDis * lightDis + b * lightDis + 1.0f);

	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float lighting = diffuse * intensity + ambient;
	return lightColor * (lighting * texture(diffuse0, texCoord) + (specular * intensity) * texture(specular0, texCoord).r);
}

vec4 directionalLight(vec3 normal, vec3 lightVec, float lightDis, vec3 lightDir, float ambient, float diffuse, float specularIntensity, int specPow) {
	// This light casts a big light, which simulates rays which are mostly parallel to each other.
	// Can be used for sunlight / natural light.

	// Lighting //
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float lighting = diffuse + ambient;
	return lightColor * (lighting * texture(diffuse0, texCoord) + specular * texture(specular0, texCoord).r);
}

vec4 spotLight(vec3 normal, vec3 lightVec, float lightDis, vec3 lightDir, float ambient, float diffuse, float specularIntensity, int specPow) {
	// This light casts a circle of light.

	float outerCone = 0.80f;
	float innerCone = 0.95f;

	// Lighting //
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	float lighting = diffuse * intensity + ambient;
	return lightColor * (lighting * texture(diffuse0, texCoord) + (specular * intensity) * texture(specular0, texCoord).r);
}

void main() {
	// Still need some works here with multiple lights
	
	// Normal Mapping //
	vec3 normal = normalize(texture(normal0, texCoord).rgb * 2.0f - 1.0f); 
	// Lighting //
	vec3 lightVec = lightPos - currPos;
	float lightDis = length(lightVec);
	// Lambertian
	vec3 lightDir = normalize(lightVec);
	float ambient = 0.1f;
	float diffuse = max(dot(normal, lightDir), ambient);
	// Specular
	float specularIntensity = 0.5f;
	int specPow = 64;

	FragColor = pointLight(normal, lightVec, lightDis, lightDir, ambient, diffuse, specularIntensity, specPow);
}