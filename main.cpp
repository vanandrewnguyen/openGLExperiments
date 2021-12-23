#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h> // This deals with loading textures and images
#include<glm/glm.hpp> // These three deal with matrix math in open gl
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"texture.h"

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

/*
// Vertices Coordinates [-1 to 1].
float vertices[] = {
	// positions         // colors          // tex coord
	-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
	0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
	 
};

GLuint indices[] = {
	0, 1, 2,
	0, 3, 2
}; */

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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Textures! //
	Texture sampleTex("testTex.jpeg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	sampleTex.texUnit(shaderProgram, "tex0", 0);

	// Uniforms! //
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Vsync & Depth test
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	// Transformation Variables
	float rotation = 1.0f;
	double prevTime = glfwGetTime();

	// Use a while loop to keep the window open.
	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		// Rendering Commands //
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		// Pass Scale Uniforms 
		double currTime = glfwGetTime();
		glUniform1f(uniID, 0.9 + 0.1 * sin(currTime));
		sampleTex.Bind();
		
		// Rotation for model 
		if (currTime - prevTime >= 1 / 60) {
			rotation += 0.4f;
			prevTime = currTime;
		}

		// Matrices (3D Projection)
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		float minSteps = 0.1f;
		float maxSteps = 100.0f;
		float fov = 45.0f;
		// Move the view/proj/model a bit - we move the viewer rather than the object
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(fov), (float)(windowWidth / windowHeight), minSteps, maxSteps);
		// Pass matrix uniforms
		int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		int viewLocation = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		int projLocation = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

		// Bind VAO and draw the triangle, updating it.
		VAO1.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); 

		// Process Window events.
		glfwPollEvents();
	}

	// Clean up objects.
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	sampleTex.Delete();
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
