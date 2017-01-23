#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>
#include <GL/glew.h>

class Texture
{
public:
	GLuint getTexture(GLuint texture);
	Texture(std::vector<GLchar*> texture_path);
	GLuint getTextureCount();
private:
	std::vector<GLuint> textures;
	std::vector<GLchar*> texture_paths;
	GLuint loadTexture(GLchar * texture_path);
	void loadAllTextures();
};
#endif