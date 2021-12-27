#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
    // Pass on texture for skybox
    FragColor = texture(skybox, texCoords);
}