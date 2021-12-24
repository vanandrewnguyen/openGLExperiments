#version 330 core
out vec4 FragColor;
in vec3 color; // the input variable from the vertex shader (same name and same type)
in vec2 texCoord;
in vec3 Normal;
in vec3 currPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
	vec3 normal = normalize(Normal);
	
	// Lighting //
	// Lambertian
	vec3 lightDir = normalize(lightPos - currPos);
	float ambient = 0.1f;
	float diffuse = max(dot(normal, lightDir), ambient);
	// Specular
	float specularIntensity = 0.5f;
	int specPow = 16;
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float lighting = diffuse + ambient + specular;
	FragColor = lightColor * lighting * texture(tex0, texCoord); 
}