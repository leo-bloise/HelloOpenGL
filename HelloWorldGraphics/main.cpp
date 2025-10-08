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

unsigned int bindVertexArray() {
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
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

	bindVertexArray();

	float vertices[] = {
		1.0f, 0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.1f, -0.5f, 0.0f,

		-0.8f, 0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 3,
		(void*)0
	);

	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
};