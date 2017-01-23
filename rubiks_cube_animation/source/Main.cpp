// GLEW manages function pointers to support different OpenGL drivers 
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW provides the bare necessities for rendering, creates OpenGL context/windows parameters
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "Camera.h"

//callback functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void mouse_callback(GLFWwindow* window, double x_position, double y_position);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat field_of_view = 45.0f;
bool keys[1024];

int main()
{
	Window mainWindow;
	GLFWwindow* window = mainWindow.getNewWindowObject("Rubik's Cube Demonstration");
	glfwSetKeyCallback(window, &key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, &scroll_callback);

	// Build and compile the shader program
	Shader ourShader("source/shaders/default.vs", "source/shaders/default.frag");
	#pragma region array data
	GLfloat texture_coordinates[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};
	// setup vertex data
	GLfloat vertices[] = {
		//back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		//front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		//left
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		//right
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 

		//bottom
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		//top
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	glm::vec3 cubie_positions[] = {
		//front face
		glm::vec3(0.0f,  2.0f,  1.0f),
		glm::vec3(1.0f,  2.0f,  1.0f),
		glm::vec3(2.0f,  2.0f,  1.0f),
		glm::vec3(0.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(2.0f,  1.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(1.0f,  0.0f,  1.0f),
		glm::vec3(2.0f,  0.0f,  1.0f),
		//middle layer
		glm::vec3(0.0f,  2.0f,  0.0f),
		glm::vec3(1.0f,  2.0f,  0.0f),
		glm::vec3(2.0f,  2.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(1.0f,  1.0f,  0.0f),
		glm::vec3(2.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  0.0f,  0.0f),
		//back layer
		glm::vec3(0.0f,  2.0f, -1.0f),
		glm::vec3(1.0f,  2.0f, -1.0f),
		glm::vec3(2.0f,  2.0f, -1.0f),
		glm::vec3(0.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(2.0f,  1.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(1.0f,  0.0f, -1.0f),
		glm::vec3(2.0f,  0.0f, -1.0f),
	};
	#pragma endregion array data

	//setup VBO/VAO/EBO along with vertex attributes
	#pragma region Buffer Objects
	//vertex buffer object -- used to send vertex data to the GPU
	//vertex array object - you must bind a VAO for OpenGL to actually render anything
	//^very easy to switch between different vertex/attribute configurations
	//element buffer object - stores indices (indexed drawing)
	GLuint VAO, EBO, vertex_buffer, texture_buffer;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &texture_buffer);
	//glGenBuffers(1, &EBO);
	// bind VAO before anything else.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*first parameter we set in vertexShaderSource (position = 0)
	* second parameter is size of vertex attribute
	* third - type of data
	* 4th -specify normalized data or not
	* 5th - for vertex attributes, there is no space b/w successive vertices, so specify size of one GLfloat.
	* 6th - offset of where the vertex data begins in the array
	*/
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//copy indices into a buffer that OpenGL reads
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coordinates), texture_coordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); 
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	#pragma endregion Buffer Objects

	//setup textures
	std::vector<GLchar*> texture_paths = { "images/yellow.jpg", "images/white.jpg", "images/orange.jpg", "images/red.jpg", "images/green.jpg", "images/blue.jpg" };
	Texture texture(texture_paths);

	Camera camera;
	//since the projection matrix rarely changes, set it once instead of per frame
	glm::mat4 projection = glm::perspective(glm::radians(field_of_view), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	//get user input, then update game state, then draw whatever
	#pragma region "Game Loop"
	while (!glfwWindowShouldClose(window))
	{
		//add functionality for moving in two directions
		//GLfloat currentFrame = glfwGetTime();
		//delta_time = current_frame - last_frame;
		//last_frame = current_frame;
		glfwPollEvents();
		//clearing the colour buffer - background colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//state using function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(shaderProgram);

		//remember to use the linked shader program here, the use() method enabls the linked shader program
		ourShader.Use();
	
		//we fake the camera (view) by using three different parameters
		//position of camera, where you want the camera to point (in this case the origin), and the up vector
		glm::mat4 view = camera.getUpdatedMatrix();
		
		//get location
		GLint modelLocation = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLocation = glGetUniformLocation(ourShader.Program, "view");
		GLint projectionLocation = glGetUniformLocation(ourShader.Program, "projection");
		//send matrix data to shader
		//the constant is how many matrices we want to send to the shader
		//third parameter is if we want to transpose the matrix
		//value_ptr is a fn that transforms matrices from GLM standard to OpenGL standard
		//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		//draw on context
		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 27; i++)
		{
			//need to declare and initialize not on the same line ?
			glm::mat4 model;
			model = glm::translate(model, cubie_positions[i]);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			GLint count = 0;
			//now render each face
			for (GLuint j = 0; j < 6; j++)
			{
				count += 6;
				//apply texture to each face
				//glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, texture.getTexture(j));
				glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);
				glDrawArrays(GL_TRIANGLES, 0, count);
			}
		}
		glBindVertexArray(0);

		/* the reason we use a double buffer here is as follows:
		* front buffer - contains the final output image that is shown at the screen
		* back buffer - rendering commands
		* to the user, the image would appear instantaneously
		* if we were to use a single buffer, flickering would incur b/c there would be a step by step generation
		*/
		glfwSwapBuffers(window);
	}
	#pragma endregion "Game Loop"
	
	#pragma region cleanup
	//deallocate objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &texture_buffer);
	//glDeleteBuffers(1, &EBO);
	//clean up resources
	glfwTerminate();
	#pragma endregion cleanup
	
	return 0;
}
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void scroll_callback(GLFWwindow * window, double x_offset, double y_offset) {
	if (field_of_view >= 1.0f && field_of_view <= 45.0f) {
		field_of_view -= y_offset;
	}
	else if (field_of_view <= 1.0f) {
		field_of_view = 1.0f;
	}
	else if (field_of_view >= 45.0f) {
		field_of_view = 45.0f;
	}
}
