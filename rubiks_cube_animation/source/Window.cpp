#include "Window.h"
#define WIDTH 800
#define HEIGHT 600

Window::Window() {
}

GLFWwindow * Window::getNewWindowObject(const char* title) {
	//instantiate glfw window
	glfwInit();
	setWindowHints();
	//create window object
	window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
	/*if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}*/
	glfwMakeContextCurrent(window);
	//capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//experimental features
	glewExperimental = GL_TRUE;
	//initialze glew before calling any OpenGL functions
	glewInit();
	/* if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}*/
	glViewport(0, 0, WIDTH, HEIGHT);
	//depth testing
	glEnable(GL_DEPTH_TEST);
	//backface culling so GPU doesn't need to render any triangles not seen by the viewer
	//glEnable(GL_CULL_FACE);
	return window;
}

void Window::setWindowHints() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}
