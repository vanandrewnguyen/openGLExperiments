#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	// ID, type, unit ref for texture object
	GLuint ID;
	GLenum type;
	GLuint unit;

	// Constructor
	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Functions for binding, unbinding, freeing, etc.
	void Bind();
	void Unbind();
	void Delete();
};
#endif