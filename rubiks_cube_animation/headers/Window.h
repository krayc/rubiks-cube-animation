#ifndef WINDOW_H
#define WINDOW_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

class Window
{
	public:
		Window();
		GLFWwindow* getNewWindowObject(const char* title);
	private:
		GLFWwindow* window;
		void setWindowHints();
};
#endif