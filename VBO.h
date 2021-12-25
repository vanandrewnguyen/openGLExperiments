#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

/*
Vertex buffer objects are buffers that contain vertex attribute and index data
*/

class VBO {
	public:
		// ID red of Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(GLfloat* vertices, GLsizeiptr size);

		// Functions for binding, unbinding, freeing, etc.
		void Bind();
		void Unbind();
		void Delete();
};

#endif