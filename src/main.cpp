#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>

const char* p_vertexShaderSource =
R"(#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
})";

const char* p_fragShaderSource =
R"(#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0, 0.5, 0.2, 1.0);
})";

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
		.7f, .2f, .0f,
		.2f, .2f, .0f,
		.45f, -.2f, .0f,
		-.7f, -.2f, .0f,
		-.2f, -.2f, .0f,
		-.45f, .2f, .0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int _vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexShader, 1, &p_vertexShaderSource, nullptr);
	glCompileShader(_vertexShader);

	int _shaderCompileStatus = 0;
	char _info[512] = {0};

	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &_shaderCompileStatus);
	if (!_shaderCompileStatus)
	{
		std::memset(_info, 0, sizeof(_info));
		glGetShaderInfoLog(_vertexShader, 512, nullptr, _info);
		std::cout << "Error compiling vertex shader: " << _info;
		glfwTerminate();
		return -1;
	}

	unsigned int _fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragShader, 1, &p_fragShaderSource, nullptr);
	glCompileShader(_fragShader);

	glGetShaderiv(_fragShader, GL_COMPILE_STATUS, &_shaderCompileStatus);
	if (!_shaderCompileStatus)
	{
		std::memset(_info, 0, sizeof(_info));
		glGetShaderInfoLog(_fragShader, 512, nullptr, _info);
		std::cout << "Error compiling fragment shader: " << _info;
		glfwTerminate();
		return -1;
	}

	unsigned int _shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragShader);
	glLinkProgram(_shaderProgram);

	glGetShaderiv(_fragShader, GL_LINK_STATUS, &_shaderCompileStatus);
	if (!_shaderCompileStatus)
	{
		std::memset(_info, 0, sizeof(_info));
		glGetProgramInfoLog(_shaderProgram, 512, nullptr, _info);
		std::cout << "Error linking shaders: " << _info;
		glfwTerminate();
		return -1;
	}

	glDeleteShader(_vertexShader);
	glDeleteShader(_fragShader);

	// Would this be necessary in real apps?
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(_window);

		glUseProgram(_shaderProgram);
		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}