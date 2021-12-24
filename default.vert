#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // 2nd column of variables, pos 1
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;
out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 currPos;

uniform float scale;
uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    currPos = vec3(model * vec4(aPos, 1.0f));
    
    gl_Position = camMatrix * vec4(currPos * scale, 1.0f); //vec4(aPos * scale, 1.0);
    color = aColor;
    texCoord = aTex;
    Normal = aNormal;
}