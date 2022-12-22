#pragma once
#include"Camera.h"
#include"deltaTime.h"
float lastX, lastY;
bool fisrtMouse = true;//抛弃第一帧标记
bool mousepressed = false;
class homework3 : public Window {
public:
	homework3(const char* title):Window(title)
	{
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	~homework3()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		delete pshader;
	}
	
	inline void RenderScreen() override final
	{
		glfwPollEvents();//告诉GLFW检查所有等待处理的事件和消息，包括操作系统和窗口系统中应当处理的消息。包括鼠标、键盘的信息。
										//如果有消息正在等待，它会先处理这些消息再返回；否则该函数会立即返回。
		glClearColor(0.3f, 0.0f, 0.5f, 1.0f);//设置清楚屏幕时所要用的颜色
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清楚颜色和深度缓冲中所有颜色，相当于设置背景色
		this->pshader->UseProgram();		//开始用着色器

	
		float timeValue = glfwGetTime();          //随时间改变颜色
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		this->pshader->Set4Float("Color", 0.0f, greenValue, 0.0f, 1.0f);
		//***********************
		//矩阵变换
		glm::mat4 transform = glm::mat4(1.0f);
		
		
		transform = glm::rotate(transform, timeValue * glm::radians(30.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f)); //缩小十倍
		transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
		this-> pshader->SetMatrix4fv("transform", transform);
		//**************************
		/*
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	*/

		//画cube
		
		this->pshaderWithColor->UseProgram();
		transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, timeValue * glm::radians(30.0f),
			glm::vec3(1.0f, 1.0f, 1.0f));      //绕对角线旋转
		this->pshaderWithColor->SetMatrix4fv("transform", transform);

		glm::mat4 proj = glm::perspective(glm::radians(pCamera ->GetZoom()), 
			static_cast<float>(WINDOWWIDTH) / static_cast<float>(WINDOWHEIGHT), 0.1f, 100.0f);
		this->pshaderWithColor->SetMatrix4fv("proj", proj);
		glm::mat4 view = pCamera->GetViewMatrix();
		this->pshaderWithColor->SetMatrix4fv("view", view);
		
		glBindVertexArray(cubeVAO);	//绑定VAO
		glDrawArrays(GL_TRIANGLES, 0, 36); //画出36个顶点
		glBindVertexArray(0); //解绑VAO
		
		glfwSwapBuffers(this->window);  //基于双缓存机制，渲染后台的颜色，将其转到前台显示。交换缓冲区
	}
	void initUI()
	{
		glfwSetKeyCallback(this->window, KeyCallback);
		glfwSetFramebufferSizeCallback(this->window, FramebufferSizeCallback);
		glfwSetCursorPosCallback(this->window, MouseCallback);
		glfwSetMouseButtonCallback(this->window, mouse_button_callback);
		glfwSetScrollCallback(this->window, scroll_callback);
	}
	void CreateObjects()
	{
		this->CreateTriangle();
		this->CreateCube();
	}
	void CreateShaders()
	{
		pshader = new Shader("core.vs", "core.fs");
		pshaderWithColor = new Shader("corewithcolor.vs",
						"corewithcolor.fs");
	}
	bool IsWindowClosed()
	{
		return glfwWindowShouldClose(window);
	}

private:
	GLuint VAO = -1;
	GLuint VBO = -1;
	Shader* pshader;
	GLuint cubeVAO = -1;
	GLuint cubeVBO = -1;
	Shader* pshaderWithColor;
	static bool keys[1024];
	static  std::unique_ptr<Camera> pCamera;
	void CreateCube()
	{
		GLfloat vertices[] = { 
		 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,														// front 
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		- 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		- 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
 
		- 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,															// back
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		- 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
 
		- 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,																							// left
		- 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		- 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		- 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 - 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		- 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        0.5f,  0.5f,  0.5f,	 1.0f, 1.0f, 0.0f,																							// right
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
          0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
 
		- 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,											// bottom
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
		- 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
		- 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
 
		- 0.5f,  0.5f, -0.5f, 0.0f, 1.0f ,1.0f,															// top
          0.5f,  0.5f, -0.5f, 0.0f, 1.0f ,1.0f,
          0.5f,  0.5f,  0.5f, 0.0f, 1.0f ,1.0f,
          0.5f,  0.5f,  0.5f, 0.0f, 1.0f ,1.0f,
		- 0.5f,  0.5f,  0.5f, 0.0f, 1.0f ,1.0f,
		- 0.5f,  0.5f, -0.5f, 0.0f, 1.0f ,1.0f

		};
		/*unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3, // first Triangle
			1, 2, 3  // second Triangle
		};
		*/
		
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		
		/*
		//可以添加EBO，记得修改glDrawArrays为glDrawElements,并且修改顶点数，和添加绑定glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO),之后解绑定;
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		*/
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void CreateTriangle()
	{
		GLfloat vertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,0.0f, 1.0f, - 1.0f, 0.0f };
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key >= 0 && key < 1024) {
			if (action == GLFW_PRESS) {
				keys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				keys[key] = false;
			}
		}
		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
			pCamera->ProcessKeyboard(FORWARD, deltaTime);
		}
		if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
			pCamera->ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
			pCamera->ProcessKeyboard(LEFT, deltaTime);
		}
		if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
			pCamera->ProcessKeyboard(RIGHT, deltaTime);
		}
	}
	static void FramebufferSizeCallback(GLFWwindow
		* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	static void MouseCallback(GLFWwindow* window, double xPos, double yPos)
	{
		GLfloat xOffset, yOffset;
		if (fisrtMouse)
		{
			lastX = xPos;
			lastY = yPos;
			fisrtMouse = false;
		}
		xOffset = xPos - lastX;
		yOffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;
		pCamera->ProcessMouseMove(xOffset, yOffset);
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			mousepressed = true;
			pCamera->ProcessMousePressed(mousepressed);
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			mousepressed = false;
			pCamera->ProcessMousePressed(mousepressed);
		}
	}
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		pCamera->ProcessScroll(yoffset);		
	}
};

std::unique_ptr<Camera> homework3::pCamera = std::unique_ptr<Camera>(new
	Camera(glm::vec3(0.0f, .0f, 5.0f)));
bool homework3::keys[1024] = { false };





