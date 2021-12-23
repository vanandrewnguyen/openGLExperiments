#version 330 core
out vec4 FragColor;
in vec4 colour; // the input variable from the vertex shader (same name and same type)
in vec2 texCoord;

uniform sampler2D tex0;

void main() {
   FragColor = colour * texture(tex0, texCoord); //vec4(0.8f, 0.3f, 0.02f, 1.0f);
}