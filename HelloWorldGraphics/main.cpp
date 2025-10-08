#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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

void drawTriangle() {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
}

int main() {	
	GLFWwindow* window = configureAsCurrentAndCreateWindow();

	if (window == NULL) {
		return -1;
	}

	unsigned int vertexShader = createVertexShader();

	if (vertexShader == 0) {
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}