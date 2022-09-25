#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstring>
#include <cmath>
#include <iostream>

#include "graphics/Shader.h"
#include "graphics/ImageImpl.h"
#include "scene/Camera.h"

Scene::Camera p_mainCamera;

float p_lastCursorX = 400.0f, p_lastCursorY = 300.0f;

float p_deltaTime(0.0f);

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

	auto _cameraFrameSpeed = (p_mainCamera.GetSpeed() * p_deltaTime);
	auto _cameraPos = p_mainCamera.GetPosition();
	auto _cameraForward = p_mainCamera.GetForward();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		p_mainCamera.SetPosition(_cameraPos + _cameraForward * _cameraFrameSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		p_mainCamera.SetPosition(_cameraPos - _cameraForward * _cameraFrameSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		p_mainCamera.SetPosition(_cameraPos + glm::normalize(glm::cross(_cameraForward, Scene::Camera::WORLD_UP)) * _cameraFrameSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		p_mainCamera.SetPosition(_cameraPos - glm::normalize(glm::cross(_cameraForward, Scene::Camera::WORLD_UP)) * _cameraFrameSpeed);
	}
}

void processMouse(GLFWwindow* window, double xPos, double yPos)
{
	static bool _firstPass = true;
	if (_firstPass)
	{
		p_lastCursorX = xPos;
		p_lastCursorY = yPos;
		_firstPass = false;
	}
	float _cursorXOffset = xPos - p_lastCursorX;
	float _cursorYOffset = p_lastCursorY - yPos;
	p_lastCursorX = xPos;
	p_lastCursorY = yPos;
	
	constexpr float _sensitivity = 0.1f;
	_cursorXOffset *= _sensitivity;
	_cursorYOffset *= _sensitivity;
	
	p_mainCamera.Rotate({_cursorXOffset, _cursorYOffset});
}

void processMouseScroll(GLFWwindow* window, double xScroll, double yScroll)
{
	p_mainCamera.Zoom(yScroll);
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
	
	float _viewportWidth = 800.0f, _viewportHeight = 600.0f;
	glViewport(0, 0, _viewportWidth, _viewportHeight);
	glEnable(GL_DEPTH_TEST);
	glClearColor(.2f, .3f, .3f, .0f);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(_window, processMouse);
	glfwSetScrollCallback(_window, processMouseScroll);
	
	/*float _vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,		// Top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// Bot right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// Bot left
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f		// Top left
	};*/
	
	float _vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

	//unsigned int _EBO = 0;
	//glGenBuffers(1, &_EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);

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

	glm::mat4 _view(1.0f);

	glm::mat4 _projection(0.0f);

	glm::vec3 _cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float _frameStartTime = 0.0f;
	float _currentTime = glfwGetTime();
	while (!glfwWindowShouldClose(_window))
	{
		_frameStartTime = _currentTime;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(_window);

		ourShader.Use();
		ourShader.SetMatrix("projection", p_mainCamera.GetProjectionMatrix());
		ourShader.SetMatrix("view", p_mainCamera.GetViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _contTextureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _faceTextureID);

		glBindVertexArray(_VAO);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for (int _currentCube = 0; _currentCube < 10; _currentCube++)
		{
			glm::mat4 _model(1.0f);
			_model = glm::translate(_model, _cubePositions[_currentCube]);
			float _angle = 20.0f * _currentCube;
			_model = glm::rotate(_model, glm::radians(_angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.SetMatrix("model", _model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
		_currentTime = glfwGetTime();
		p_deltaTime = _currentTime - _frameStartTime;
	}
	
	glfwTerminate();
	
	return 0;
}