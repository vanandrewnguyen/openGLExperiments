#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // 2nd column of variables, pos 1
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;
out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 currPos;

uniform float slide; // be careful since you're not using this atm, could lead to some errors
uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    // Update current position
    currPos = vec3(model * vec4(aPos, 1.0f));
    
    // Output the variables and position
    gl_Position = camMatrix * vec4(currPos, 1.0f);
    color = aColor;
    texCoord = aTex;
    Normal = aNormal;
}