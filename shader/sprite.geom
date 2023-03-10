#version 420 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TexCoords[];
out vec2 texCoords;

void main()
{
	for (int i = 0; i < 3;  i++)
	{
		gl_Position = gl_in[i].gl_Position;
		texCoords = TexCoords[i];

		EmitVertex();
	}

	EndPrimitive();
}
