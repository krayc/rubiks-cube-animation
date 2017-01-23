#include <GL/glew.h>
// GLFW provides the bare necessities for rendering, creates OpenGL context/windows parameters
#include <SOIL.h>
#include "Texture.h"
#include <iostream>

Texture::Texture(std::vector<GLchar*> texture_path) {
	for (int i = 0; i < texture_path.size(); i++) {
		texture_paths.push_back(texture_path[i]);
	}
	textures.reserve(texture_path.size());
	loadAllTextures();
}

void Texture::loadAllTextures() {
	for (int i = 0; i < texture_paths.size(); i++) {
		std::cout << texture_paths[i];
		textures.push_back(loadTexture(texture_paths[i]));
	}
}

GLuint Texture::getTexture(GLuint texture) {
	return textures.at(texture);
}

GLuint Texture::getTextureCount() {
	return (GLuint) textures.size();
}

GLuint Texture::loadTexture(GLchar * texturePath) {
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