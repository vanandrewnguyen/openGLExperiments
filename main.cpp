#include<iostream>
#include"mesh.h"

// Vertices coordinates (Plane)
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /       NORMALS              /       TexCoord         //
	Vertex{glm::vec3(-1.0f, -0.5f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, -0.5f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, -0.5f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, -0.5f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

float lSize = 0.05f;
Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-lSize, -lSize, lSize)},
	Vertex{glm::vec3(-lSize, -lSize, -lSize)},
	Vertex{glm::vec3(lSize, -lSize, -lSize)},
	Vertex{glm::vec3(lSize, -lSize, lSize)},
	Vertex{glm::vec3(-lSize, lSize, lSize)},
	Vertex{glm::vec3(-lSize, lSize, -lSize)},
	Vertex{glm::vec3(lSize, lSize, -lSize)},
	Vertex{glm::vec3(lSize, lSize, lSize)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
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
	const char* windowName = "OpenGL Test";
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

	// Textures! //
	// Import textures in an array
	Texture textures[]{
		Texture("iceTexture.jpeg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("iceSpecTexture.jpeg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Create mesh data using vectors using textures, vertices, indices arrays from above
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);

	// Lights! //
	Shader lightShader("light.vert", "light.frag");
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Crate light mesh
	Mesh light(lightVerts, lightInd, tex);

	// We create light position, colours, model and pass it all to the light shader.
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Uniforms! //
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "slide");

	// Vsync & Depth test
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	// Camera! //
	Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

	// Use a while loop to keep the window open.
	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		// Rendering Commands //
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //0.07f, 0.13f, 0.17f
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Camera
		float minSteps = 0.1f;
		float maxSteps = 100.0f;
		float fov = 45.0f;
		camera.Inputs(window);
		camera.updateMatrix(fov, minSteps, maxSteps);

		// Draw meshes
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);

		// Update (by swapping each frame)
		glfwSwapBuffers(window); 

		// Process Window events.
		glfwPollEvents();
	}

	// Clean up objects.
	shaderProgram.Delete();
	lightShader.Delete();
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
