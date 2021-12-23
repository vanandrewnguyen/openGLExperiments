#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColour; // 2nd column of variables, pos 1
layout (location = 2) in vec2 aTex;
out vec4 colour;
out vec2 texCoord;

uniform float scale;

void main() {
   gl_Position = vec4(aPos * scale, 1.0);
   colour = vec4(aColour, 1.0);
   texCoord = aTex;
}