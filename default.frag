#version 330 core
out vec4 FragColor;
in vec3 color; // the input variable from the vertex shader (same name and same type)
in vec2 texCoord;
in vec3 Normal;
in vec3 currPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight() {
	// This light dims after a certain extent, which is modulated using intensity with the inverse square law.
	// Lights objects in all directions equally.

	vec3 normal = normalize(Normal);

	// Lighting //
	vec3 lightVec = lightPos - currPos;
	float lightDis = length(lightVec);

	// Light Eqn, inverse square law
	float a = 0.5;
	float b = 0.2;
	float intensity = 1.0f / (a * lightDis * lightDis + b * lightDis + 1.0f);

	// Lambertian
	vec3 lightDir = normalize(lightVec);
	float ambient = 0.1f;
	float diffuse = max(dot(normal, lightDir), ambient);
	// Specular
	float specularIntensity = 0.5f;
	int specPow = 64;
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float lighting = diffuse * intensity + ambient;
	return lightColor * (lighting * texture(tex0, texCoord) + (specular * intensity) * texture(tex1, texCoord).r);
}

vec4 directionalLight() {
	// This light casts a big light, which simulates rays which are mostly parallel to each other.
	// Can be used for sunlight / natural light.

	vec3 normal = normalize(Normal);
	
	// Lighting //
	// Lambertian
	vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f)); // this vec3 is a magic number
	float ambient = 0.1f;
	float diffuse = max(dot(normal, lightDir), ambient);
	// Specular
	float specularIntensity = 0.5f;
	int specPow = 64;
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float lighting = diffuse + ambient;
	return lightColor * (lighting * texture(tex0, texCoord) + specular * texture(tex1, texCoord).r);
}

vec4 spotLight() {
	// This light casts a circle of light.

	vec3 normal = normalize(Normal);
	float outerCone = 0.80f;
	float innerCone = 0.95f;

	// Lighting //
	// Lambertian
	vec3 lightDir = normalize(lightPos - currPos);
	float ambient = 0.1f;
	float diffuse = max(dot(normal, lightDir), ambient);
	// Specular
	float specularIntensity = 0.5f;
	int specPow = 64;
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	float lighting = diffuse * intensity + ambient;
	return lightColor * (lighting * texture(tex0, texCoord) + (specular * intensity) * texture(tex1, texCoord).r);
}

void main() {
	FragColor = pointLight();
}