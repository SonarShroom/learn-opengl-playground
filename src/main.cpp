#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstring>
#include <cmath>
#include <iostream>

#include "graphics/Shader.h"
#include "graphics/ImageImpl.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "Error initializing GLFW. Exiting.";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

	GLFWwindow* _window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (!_window)
	{
		std::cout << "Failed to create GLFW window. Exiting.";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load OpenGL functions with GLAD. Exiting.";
		glfwTerminate();
		return -1;
	}
	
	glViewport(0, 0, 800, 600);
	glClearColor(.2f, .3f, .3f, .0f);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	
	float _vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// Top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// Bot right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// Bot left
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f		// Top left
	};
	unsigned int _indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int _VAO = 0;
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	unsigned int _VBO = 0;
	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	unsigned int _EBO = 0;
	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	stbi_set_flip_vertically_on_load(true);
	unsigned int _contTextureID = 0;
	int _width = 0;
	int _height = 0;
	int _nChannels = 0;
	unsigned char* _contTexData = stbi_load("images/container.jpg", &_width, &_height, &_nChannels, 0);

	if (_contTexData)
	{
		glGenTextures(1, &_contTextureID);
		glBindTexture(GL_TEXTURE_2D, _contTextureID); // Binds to the currently active texture unit
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _contTexData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load the texture data in images/container.jpg.\n";
	}
	stbi_image_free(_contTexData);
	
	unsigned int _faceTextureID = 0;
	unsigned char* _faceTexData = stbi_load("images/awesomeface.png", &_width, &_height, &_nChannels, 0);

	if (_faceTexData)
	{
		glGenTextures(1, &_faceTextureID);
		glBindTexture(GL_TEXTURE_2D, _faceTextureID); // Binds to the currently active texture unit
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _faceTexData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load the texture data in images/awesomeface.png.\n";
	}
	stbi_image_free(_faceTexData);

	Graphics::Shader ourShader("shaders/vertShader.vert", "shaders/fragShader.frag");
	ourShader.Use();
	ourShader.SetInt("Texture1", 0);
	ourShader.SetInt("Texture2", 1);

	// Would this be necessary in real apps?
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(_window);

		ourShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _contTextureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _faceTextureID);

		glBindVertexArray(_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}