#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
	
	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(_window);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return 0;
}