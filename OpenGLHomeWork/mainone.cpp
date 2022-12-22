
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Minicore.h"
#include "Windows.h"
#include "homework1.h"




int main()
{
	homework1 tutorial("B20030119");
	//tutorial.InitWindow("homework34");
	tutorial.initUI();
	tutorial.CreateObjects();
	tutorial.CreateShaders();
	float currentTime = 0.0f;
	float lastTime = 0.0f;
	//extern float deltaTime;
	while (!tutorial.IsWindowClosed())
	{
		tutorial.RenderScreen();
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		tutorial.RenderScreen();
	}

	return 0;
}