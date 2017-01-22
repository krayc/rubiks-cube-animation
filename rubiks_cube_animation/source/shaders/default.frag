//fragment shader
#version 330 core
//in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
//uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// use both textures and output and linearly interpolate based on last argument
	color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
	//color = texture(ourTexture, TexCoord);
}