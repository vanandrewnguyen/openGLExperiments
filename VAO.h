#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

/*
A vertex array object is needed so OpenGL knows what to do with vertex inputs, otherwise it won't draw anything.
VAO's are like structs / wrappers of draw calls, pointers to VBO's. 
*/

class VAO {
	public:
		// ID ref for Vertex Array Object
		GLuint ID;
		// Constructor that generates a VAO ID
		VAO();

		// Links a VBO to the VAO using a certain layout
		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		// Functions for binding, unbinding, freeing, etc.
		void Bind();
		void Unbind();
		void Delete();
};
#endif