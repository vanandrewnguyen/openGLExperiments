#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

/*
The EBO is a buffer like the VBO, which stores indicies that OpenGL uses to decide which vertices to draw.
*/

class EBO {
	public:
		// ID ref for "Elements Buffer Object"
		GLuint ID;
		// Constructor that generates a Elements Buffer Object and links it to indices
		EBO(GLuint* indices, GLsizeiptr size);

		// Functions for binding, unbinding, freeing, etc.
		void Bind();
		void Unbind();
		void Delete();
};

#endif