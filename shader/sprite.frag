#version 420 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
	color = texture(image, texCoords);
	//color = vec4(0.0, 0.0, 0.0, 1.0);
}
