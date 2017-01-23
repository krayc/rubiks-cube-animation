#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	void updateCamera();
	glm::mat4 getUpdatedMatrix();
private:
	GLfloat camera_x;
	GLfloat camera_y;
	GLfloat camera_z;
	glm::vec3 camera_position; 
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	GLfloat delta_time; 
	GLfloat last_frame; 
	glm::mat4 view;
};
#endif