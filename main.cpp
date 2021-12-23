#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

// Vertices Coordinates [-1 to 1].
float vertices[] = {
	// positions         // colors          // tex coord
	-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
	 
};

GLuint indices[] = {
	0, 1, 2,
	0, 3, 2
};

// Function Definition
void processInput(GLFWwindow* window);

// Main
int main() {
	// Init GLFW Library.
	glfwInit();

	// Tell GLFW which versions of OpenGL we will use, and which profile to use.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window creation. 
	int windowHeight = 800;
	int windowWidth = 800;
	const char* windowName = "Hello World!";
	GLFWwindow* window = glfwCreateWindow(windowHeight, windowWidth, windowName, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create a GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set window as target.
	glfwMakeContextCurrent(window);

	// Load OpenGL.
	gladLoadGL();
	glViewport(0, 0, windowHeight, windowWidth);

	// Call on header files for shader classes.
	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Textures! //
	int imgW, imgH, numColCh;
	unsigned char* bytes = stbi_load("sampleTex.png", &imgW, &imgH, &numColCh, 0);
	// Create tex obj
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Tex settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Generate tex
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Free tex data
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Uniforms! //
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	GLuint uniTex0 = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(uniTex0, 0);


	// Use a while loop to keep the window open.
	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		// Rendering Commands //
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();

		// Pass Uniforms 
		float timeValue = glfwGetTime();
		glUniform1f(uniID, 0.5 + 0.2 * sin(timeValue));
		glBindTexture(GL_TEXTURE_2D, texture);

		// Bind VAO and draw the triangle, updating it.
		VAO1.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); 

		// Process Window events.
		glfwPollEvents();
	}

	// Clean up objects.
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

	// Terminate window and end program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// Window Input function, exits program if ESC is pressed.
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}