#version 330 core
// These are seen on main.cpp as a big vertex vector (not an array since undefined length)
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;
// Output values
out vec3 currPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
// Matrix uniforms
uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    // Update current position
    currPos = vec3(model * vec4(aPos, 1.0f));
    
    // Output values
    color = aColor;
    texCoord = aTex;
    Normal = aNormal;

    // Output the variables and position
    gl_Position = camMatrix * vec4(currPos, 1.0f);
}