#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

// Grab file name
std::string get_file_contents(const char* filename);

class Shader {
	public:
		// Ref ID for shader object
		GLuint ID;
		
		// Set geometry and pixel shader from file names
		Shader(const char* vertexFile, const char* fragmentFile);

		// Functions for initialising and freeing
		void Activate();
		void Delete();
	private:
		void compileErrors(unsigned int shader, const char* type);
};

#endif