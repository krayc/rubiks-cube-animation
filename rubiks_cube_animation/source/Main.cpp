// GLEW manages function pointers to support different OpenGL drivers 
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW provides the bare necessities for rendering, creates OpenGL context/windows parameters
#include <GLFW/glfw3.h>
#include <iostream>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

//functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint load_texture(const GLchar * texturePath);
void loadAllTextures();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat fov = 45.0f;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	//difference in time b/w current and last frame
GLfloat lastFrame = 0.0f; 

GLuint textures[6];

int main()
{
	#pragma region setup window

	//instantiate glfw window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//create window object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rubik's Cube demonstration", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// set call back functions
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//initialze glew before calling any OpenGL functions
	//glewExperimental is for using experimental functions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	//set viewport size
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//processed coordinates are in the range of [-1, 1]
	//in this example, (-1, 1) maps to (0, 800) and (0, 600)
	glViewport(0, 0, WIDTH, HEIGHT);
	//depth testing
	glEnable(GL_DEPTH_TEST);
	#pragma endregion setup window
	
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


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	#pragma endregion Buffer Objects

	loadAllTextures();

	//since the projection matrix rarely changes set it once instead of per frame
	
	#pragma region "Game Loop"
	while (!glfwWindowShouldClose(window))
	{
		//add functionality for moving in two directions
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//get user input, then update game state, then draw whatever
		glfwPollEvents();
		do_movement();
		// Rendering commands here
		//clearing the colour buffer
		//state setting function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//state using function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(shaderProgram);

		//remember to use the linked shader program here, the use() method enabls the linked shader program
		ourShader.Use();


		//glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Note that we're translating the scene in the reverse direction of where we want to move
		//the view matrix is the camera
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//switch around subtraction order for the vector to point to +ve z-axis
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		//vector that points to +ve x-axis
		//glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
		//vector that points to +ve y-axis
		//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		//glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 5.5f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//projection matrix
		//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		
		//glm::mat4 model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		//glm::mat4 model = glm::rotate(model, -15.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
		// Camera/View transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// projection matrix 
		//remember to convert to radians
		glm::mat4 projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

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
			glm::mat4 model;
			model = glm::translate(model, cubie_positions[i]);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			GLint count = 0;
			//now render each face
			for (GLuint j = 0; j < 6; j++)
			{
				//apply texture to each face
				//glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, textures[j]);
				glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);
				glDrawArrays(GL_TRIANGLES, 0, count);
				count += 6;
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}	
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_S])
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_A])
	{
		cameraPos -= glm::cross(cameraFront, cameraUp) * cameraSpeed;
	}
	if (keys[GLFW_KEY_D])
	{
		cameraPos += glm::cross(cameraFront, cameraUp) * cameraSpeed;
	}	
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
	{
		fov -= yoffset;
	}
	else if (fov <= 1.0f)
	{
		fov = 1.0f;
	}
	else if (fov >= 45.0f)
	{
		fov = 45.0f;
	}
}

void loadAllTextures()
{
	textures[0] = load_texture("images/yellow.jpg");
	textures[1] = load_texture("images/white.jpg");
	textures[2] = load_texture("images/orange.jpg");
	textures[3] = load_texture("images/red.jpg");
	textures[4] = load_texture("images/green.jpg");
	textures[5] = load_texture("images/blue.jpg");
}


GLuint load_texture(const GLchar * texturePath)
{
	GLuint texture;
	glGenTextures(1, &texture);
	//bind texture to 2D, so that any 2D manipulation will affect this texture
	glBindTexture(GL_TEXTURE_2D, texture);
	//texture wrapping s,t,r (equivalent to x,y,z)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// texture filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// image setup
	int width, height;
	unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//mipmap generation
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind
	return texture;
}
