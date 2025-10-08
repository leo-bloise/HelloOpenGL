#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

bool checkCompilationShaderSuccess(unsigned int shaderId) {
	int success;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success) {
		char info[512];
		glGetShaderInfoLog(shaderId, 512, NULL, info);
		std::cerr << "SHADER COMPILATION ERROR: " << info;
		return false;
	}

	return true;
}

unsigned int createFragmentShader() {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	if (!checkCompilationShaderSuccess(fragmentShader)) {
		return 0;
	}

	return fragmentShader;
}

unsigned int createVertexShader() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	if (!checkCompilationShaderSuccess(vertexShader)) {
		return 0;
	}

	return vertexShader;
}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		char info[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cerr << "ERROR LINKING SHADER PROGRAM: " << info;
		return 0;
	}

	return shaderProgram;
}

GLFWwindow* configureAsCurrentAndCreateWindow() {
	if (!glfwInit()) {
		std::cerr << "ERROR WHILE INITIATING GLFW";
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World OpenGL", NULL, NULL);
	
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return NULL;
	}

	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 800);

	GLenum err = glewInit();

	if (err != GLEW_OK) {
		std::cerr << "Error while init glew: " << glewGetErrorString(err);
		return NULL;
	}

	return window;
}

int main() {	
	GLFWwindow* window = configureAsCurrentAndCreateWindow();

	if (window == NULL) {
		return -1;
	}
	// DETERMING AND BINDING SHADER PROGRAM..
	unsigned int vertexShader = createVertexShader();

	if (vertexShader == 0) {
		return -1;
	}

	unsigned int fragmentShader = createFragmentShader();

	if (fragmentShader == 0) {
		return -1;
	}

	unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);
	// DETERMING AND BINDING SHADER PROGRAM..

	int retries = 0;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,-0.5f,0.0f,
		-0.5f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int EBO;
	unsigned int VBO;
	
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, indices, GL_STATIC_DRAW);

	glVertexAttribPointer( // It determines how to read a buffer data in opengl, so one vertex buffer object must be attached to it.
		0, // Attribute position in the shader
		3, // How many items one vertex has 
		GL_FLOAT, // What is the type of it?
		GL_FALSE, // Normalize values to -1, 0 or 1. (In this case, they're already normalized)
		3 * sizeof(float), // How many bytes one vertex occupies (It determines the jump to the next vertex).
		(void*)0 // From where inside the vertex buffer object open gl must start reading
	);

	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
};