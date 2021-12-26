#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

/*
Vertex buffer objects are buffers that contain vertex attribute and index data
*/

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO {
	public:
		// ID red of Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(std::vector<Vertex>& vertices);

		// Functions for binding, unbinding, freeing, etc.
		void Bind();
		void Unbind();
		void Delete();
};

#endif