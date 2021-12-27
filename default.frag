#version 330 core
out vec4 FragColor;
in vec3 currPos; // the input variable from the vertex shader (same name and same type)
in vec3 Normal;
in vec3 color; 
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform sampler2D displacement0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight(vec3 normal, vec3 lightVec, float lightDis, vec3 lightDir, float ambient, float diffuse, float specularIntensity, int specPow, vec2 UV) {
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
	return lightColor * (lighting * texture(diffuse0, UV) + (specular * intensity) * texture(specular0, UV).r);
}

vec4 directionalLight(vec3 normal, vec3 lightVec, float lightDis, vec3 lightDir, float ambient, float diffuse, float specularIntensity, int specPow, vec2 UV) {
	// This light casts a big light, which simulates rays which are mostly parallel to each other.
	// Can be used for sunlight / natural light.

	// Lighting //
	vec3 viewDir = normalize(camPos - currPos);
	vec3 reflectionDir = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), specPow);
	float specular = specAmount * specularIntensity;

	float lighting = diffuse + ambient;
	return lightColor * (lighting * texture(diffuse0, UV) + specular * texture(specular0, UV).r);
}

vec4 spotLight(vec3 normal, vec3 lightVec, float lightDis, vec3 lightDir, float ambient, float diffuse, float specularIntensity, int specPow, vec2 UV) {
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
	return lightColor * (lighting * texture(diffuse0, UV) + (specular * intensity) * texture(specular0, UV).r);
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

	// Parallax Occlusion Mapping // Source: https://learnopengl.com/Advanced-Lighting/Parallax-Mapping
	vec3 viewDir = normalize(camPos - currPos);
	// Variables that control parallax occlusion mapping quality
	float heightScale = 0.05f;
	const float minLayers = 8.0f;
    const float maxLayers = 64.0f;
	// The split in layers creates steps where you march inwards until you hit something, i.e. multiple layers
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));
	float layerDepth = 1.0f / numLayers;
	float currentLayerDepth = 0.0f;
	// Remove the z division if you want less aberated results
	vec2 S = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaUVs = S / numLayers;
	// Init UV 
	vec2 UV = texCoord;
	float currentDepthMapValue = 1.0f - texture(displacement0, UV).r;
	// Loop till the point on the heightmap is "hit"
	while(currentLayerDepth < currentDepthMapValue) { // kinda like a ray march but through the heightmap
        UV -= deltaUVs;
        currentDepthMapValue = 1.0f - texture(displacement0, UV).r;
        currentLayerDepth += layerDepth;
    }
	// Apply Occlusion (interpolation with prev value)
	vec2 prevTexCoords = UV + deltaUVs;
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = 1.0f - texture(displacement0, prevTexCoords).r - currentLayerDepth + layerDepth;
	float weight = afterDepth / (afterDepth - beforeDepth);
	UV = prevTexCoords * weight + UV * (1.0f - weight);
	// Get rid of anything outside the normal range
	if(UV.x > 1.0 || UV.y > 1.0 || UV.x < 0.0 || UV.y < 0.0) {
		discard;
	}

	FragColor = pointLight(normal, lightVec, lightDis, lightDir, ambient, diffuse, specularIntensity, specPow, UV);
}