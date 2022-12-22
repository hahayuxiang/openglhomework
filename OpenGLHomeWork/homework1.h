#pragma once
#include <vector>
#include"Camera.h"
#include"deltaTime.h"
#include"Point.h"
#include"stb_image.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
float lastX, lastY;
bool fisrtMouse = true;//抛弃第一帧标记
bool mousepressed = false;
class homework1 : public Window {
public:
	homework1(const char* title) : Window(title)
	{
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	~homework1()
	{
		for (int i = 0; i < 5; i++) {
			glDeleteVertexArrays(1, &VAO[i]);
			glDeleteBuffers(1, &VBO[i]);

		}
		
		//glDeleteBuffers(1, &EBO);
		delete pshader;
	}

	inline void RenderScreen() override final
	{
		glfwPollEvents();//告诉GLFW检查所有等待处理的事件和消息，包括操作系统和窗口系统中应当处理的消息。包括鼠标、键盘的信息。
		//如果有消息正在等待，它会先处理这些消息再返回；否则该函数会立即返回。
		glClearColor(0.878588f, 0.160784f, 0.062745f, 1.0f);//设置清楚屏幕时所要用的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清楚颜色和深度缓冲中所有颜色，相当于设置背景色
		this->pshader->UseProgram();		//开始用着色器


		
		this->pshader->Set4Float("Ourcolor", 1.0f, 0.870588f, 0.0f, 1.0f);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);//画大五角星
		
		for (int i = 1; i < 5; i++) {
			float timeValue = glfwGetTime() + i;
			float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
			this->pshader->Set4Float("Ourcolor", 1.0f, 0.870588f, 0.0f, 1.0f);
			glBindVertexArray(VAO[i]);
			glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);//分别画其余五个小五角星
		}


		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(0);

		glfwSwapBuffers(this->window);
		glfwPollEvents();

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
		this->CreateCube();
	}
	void CreateShaders()
	{
		pshader = new Shader("coreone.vs", "coreone.fs");
		
	}
	bool IsWindowClosed()
	{
		return glfwWindowShouldClose(window);
	}

private:
	GLuint VAO[5];
	GLuint VBO[5];
	GLuint EBO[5];
	GLuint cubeVAO = -1;
	GLuint cubeVBO = -1;
	Shader* pshaderWithColor;

	Shader* pshader;

	static bool keys[1024];
	static std::unique_ptr<Camera> pCamera;

	int num = 30;


	void CreateCube()
	{
		float vertices[][33]{
		{
			-0.666667f, 0.6f,		0.0f,

			-0.666667f, 0.8f,		0.0f,
			-0.856878f, 0.592705f,	0.0f,
			-0.784224f, 0.257295f,	0.0f,
			-0.54911f,  0.257295f,	0.0f,
			-0.476455f, 0.592705f,	0.0f,

			-0.711573f, 0.592713,	0.0f,
			-0.739327f, 0.464587,	0.0f,
			-0.666667f, 0.3854,		0.0f,
			-0.594006f, 0.464587,	0.0f,
			-0.62176f,  0.592713,	0.0f
		},
		{
			-0.333f,	0.8f,		0.0f,

			-0.3905f,	0.74855f,	0.0f,
			-0.318378f,	0.702549f,	0.0f,
			-0.266924f, 0.791221f,	0.0f,
			-0.307246f, 0.892026f,	0.0f,
			-0.38362f,	0.865654f,	0.0f,

			-0.343299f, 0.764846f,	0.0f,
			-0.314124f, 0.77492f,	0.0f,
			-0.311496f,	0.819654f,	0.0f,
			-0.339046f, 0.837226f,	0.0f,
			-0.358702f, 0.803353f,	0.0f
		},
		{
			-0.2f,		0.6f,		0.0f,

			-0.265997f, 0.585858f,	0.0f,
			-0.211427f, 0.50148f,	0.0f,
			-0.141066f, 0.553253f,	0.0f,
			-0.152149f, 0.669629f,	0.0f,
			-0.229361f, 0.68978f,	0.0f,

			-0.218279f, 0.573402f,	0.0f,
			-0.188784f, 0.565704f,	0.0f,
			-0.174789f, 0.605402f,	0.0f,
			-0.195635f, 0.637635f,	0.0f,
			-0.222513f, 0.617857f,	0.0f
		},
		{
			-0.2f,		0.3f,		0.0f,

			-0.264102f, 0.327472f,	0.0f,
			-0.237227f, 0.217043f,	0.0f,
			-0.158906f, 0.221258f,	0.0f,
			-0.137376f, 0.334292f,	0.0f,
			-0.20239f,  0.399936f,	0.0f,

			-0.223923f, 0.286901f,	0.0f,
			-0.199087f, 0.261825f,	0.0f,
			-0.175513f, 0.289506f,	0.0f,
			-0.185779f, 0.33169f,	0.0f,
			-0.215698f, 0.33008f,	0.0f
		},
		{
			-0.333f,	0.1f,		0.0f,

			-0.385391f, 0.16247f,	0.0f,
			-0.389028f, 0.0450391f,	0.0f,
			-0.315697f, 0.00356278f,0.0f,
			-0.266739f, 0.0953593f, 0.0f,
			-0.309812f, 0.193569f,	0.0f,

			-0.358773f, 0.101773f,	0.0f,
			-0.342318f, 0.0642566f,	0.0f,
			-0.313447f, 0.0761366f, 0.0f,
			-0.312058f, 0.120995f,	0.0f,
			-0.340071f, 0.136839f,	0.0f
		}

		};
		unsigned int indices[] = {
			0, 6, 2,
			0, 2, 7,
			0, 1, 6,
			0, 1, 10,
			0, 5, 10,
			0, 5, 9,
			0, 4, 9,
			0, 4, 8,
			0, 3, 7,
			0, 3, 8
		};
		/*float vertices[]{
			 0.5f,  0.5f, 0.0f,	//右上角
			 0.5f, -0.5f, 0.0f,	//右下角
			-0.5f,  0.5f, 0.0f, //左上角
			-0.5f, -0.5f, 0.0f  //左下角
		};*/
		/*unsigned int indices[] = {
			0, 1, 2,
			3, 1, 2
		};*/

		//顶点缓冲
		
		for (int i = 0; i < 5; i++) {
			glGenVertexArrays(1, &VAO[i]);
			glGenBuffers(1, &VBO[i]);
			glGenBuffers(1, &EBO[i]);

			glBindVertexArray(VAO[i]);

			glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}
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

std::unique_ptr<Camera> homework1::pCamera = std::unique_ptr<Camera>(new
	Camera(glm::vec3(0.0f, -5.0f, 0.0f)));
bool homework1::keys[1024] = { false };





