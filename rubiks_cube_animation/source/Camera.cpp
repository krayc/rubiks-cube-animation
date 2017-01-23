#include "Camera.h"

Camera::Camera() {
	this->camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
	this->camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

//control camera via WASD
void Camera::updateCamera() {
	GLfloat camera_speed = 5.0f * delta_time;
	/*if (keys[GLFW_KEY_W])
	{
		camera_position += camera_speed * camera_front;
	}
	if (keys[GLFW_KEY_S])
	{
		camera_position -= camera_speed * camera_front;
	}
	if (keys[GLFW_KEY_A])
	{
		camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	}
	if (keys[GLFW_KEY_D])
	{
		camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	}*/
}

glm::mat4 Camera::getUpdatedMatrix()
{
	GLfloat radius = 10.0f;
	camera_x = sin(glfwGetTime()) * radius;
	camera_y = cos(glfwGetTime()) * sin(glfwGetTime()) * radius;
	camera_z = cos(glfwGetTime()) * radius;
	view = glm::lookAt(glm::vec3(camera_x, camera_y, camera_z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
	return view;
}
