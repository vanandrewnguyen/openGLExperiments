#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

/*
Mesh object combines all the different textures / cameras / vertex/indices data / shaders into one class for easy calls.
Abstraction makes it easier to deal with esp with model importing.
*/

class Mesh {
	public:
		// These are defined from main.cpp
		std::vector <Vertex> vertices;
		std::vector <GLuint> indices;
		std::vector <Texture> textures;
		VAO VAO;

		// Mesh constructor
		Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

		// Draws the mesh with default values
		void Draw
		(
			Shader& shader,
			Camera& camera,
			glm::mat4 matrix = glm::mat4(1.0f),
			glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
		);
};
#endif