#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <cglm/call.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static const GLfloat vertices[] =
{ 
	// Pos
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,

	// Tex Coords
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
};

static const GLuint indices[] =
{
	0, 1, 2,
	1, 2, 3,
};


GLchar* readFile(const char* fileName)
{
	FILE* file = fopen(fileName, "rb");
	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	GLchar* string = malloc(size + 1);
	fseek(file, 0L, SEEK_SET);
	fread(string, sizeof(char), size, file);
	string[size] = 0;
	fclose(file);
	return string;
}

/* channel
{0: R} {1: G} {2: B} {3: A} */
uint8_t getPixelValue(void* data, uint32_t width, uint32_t x, uint32_t y, uint8_t channel)
{
	return ((uint8_t*)data)[(y * width + x) * 4 + channel];
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Gaussian Blur", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();

	glViewport(0, 0, 800, 600);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mat4 projection;
	glmc_ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f, projection);

	// Shader
	GLuint shader, VShader, GShader, FShader;
	GLchar* code;

	VShader = glCreateShader(GL_VERTEX_SHADER);
	code = readFile("../shader/sprite.vert");
	glShaderSource(VShader, 1, (const GLchar**)&code, NULL);
	glCompileShader(VShader);
	free(code);

	GShader = glCreateShader(GL_GEOMETRY_SHADER);
	code = readFile("../shader/sprite.geom");
	glShaderSource(GShader, 1, (const GLchar**)&code, NULL);
	glCompileShader(GShader);
	free(code);

	FShader = glCreateShader(GL_FRAGMENT_SHADER);
	code = readFile("../shader/sprite.frag");
	glShaderSource(FShader, 1, (const GLchar**)&code, NULL);
	glCompileShader(FShader);
	free(code);

	shader = glCreateProgram();
	glAttachShader(shader, VShader);
	glAttachShader(shader, GShader);
	glAttachShader(shader, FShader);
	glLinkProgram(shader);
	glDeleteShader(VShader);
	glDeleteShader(FShader);
	glDeleteShader(GShader);

	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "image"), 0);

	// Quad VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * 0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * 8));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Texture
	int channels;
	int width, height;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	stbi_uc* data = stbi_load("../res/image2.png", &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);


	printf("R: %03u\nG: %03u\nB: %03u\nA: %03u",
		getPixelValue(data, width, 7, 0, 0),
		getPixelValue(data, width, 7, 0, 1),
		getPixelValue(data, width, 7, 0, 2),
		getPixelValue(data, width, 7, 0, 3)
	);

	stbi_image_free(data);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAW THE FUCKING SHIT
		mat4 model;
		glmc_mat4_identity(model);
		vec3 scale = {600.0f, 600.0f, 1.0f};
		glmc_scale(model, scale);

		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, (GLfloat*)model);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, (GLfloat*)projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
		glBindVertexArray(0);
		//

		glfwSwapBuffers(window);
	}

	return 0;
}