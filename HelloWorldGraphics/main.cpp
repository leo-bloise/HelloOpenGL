#include <iostream>
#include <GLFW/glfw3.h>

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

	return window;
}

int main() {	
	GLFWwindow* window = configureAsCurrentAndCreateWindow();

	if (window == NULL) {
		return -1;
	}

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}