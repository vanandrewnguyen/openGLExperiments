#include<iostream>
#include"mesh.h"

// Vertices coordinates (Plane)
float pSize = 4.0f;
float pDepth = 2.0f;
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /       NORMALS              /       TexCoord         //
	Vertex{glm::vec3(-pSize, -pDepth,  pSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-pSize, -pDepth, -pSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(pSize, -pDepth, -pSize),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(pSize, -pDepth,  pSize),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

float lSize = 0.1f;
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

// Vertices coordinates (Skybox)
float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
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

	// Cubemaps! //
	// Create seperate shader class for skybox
	Shader skyboxShader("skybox.vert", "skybox.frag");
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
	// Create VAO, VBO, and EBO for the skybox, identical to mesh.h
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// All the faces of the cubemap 
	std::string facesCubemap[6] =
	{
		"px.jpeg",
		"nx.jpeg",
		"py.jpeg",
		"ny.jpeg",
		"pz.jpeg",
		"nz.jpeg"
	};
	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// Cycles through all the textures and attaches them to the cubemap object
	// Cubemaps start in top left corner not bottom left corner
	for (unsigned int i = 0; i < 6; i++) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		} else {
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}

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

		// Draw skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(fov), (float)windowWidth / windowHeight, minSteps, maxSteps);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);


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
