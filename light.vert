#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix;

void main() {
	// Basic pass, just combine the model with camera matrices 
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}