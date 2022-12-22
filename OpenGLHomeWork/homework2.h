#pragma once
#include <vector>
#include"cameratwo.h"
#include"deltaTime.h"
#include"Point.h"
#include"stb_image.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
// Window dimensions
GLuint WIDTH = 800, HEIGHT = 600;

const double PI = 3.1415926535;
int ustepNum = 100, vstepNum = 50;
float lastX = 400, lastY = 300;
bool fisrtMouse = true;//抛弃第一帧标记
bool mousepressed = false;






class homework2 : public Window {
public:
	homework2(const char* title) :Window(title)
	{
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	~homework2()
	{
		for (int i = 0; i < 3; i++) {
			glDeleteVertexArrays(1, &VAO[i]);
			glDeleteBuffers(1, &VBO[i]);
			glDeleteBuffers(1, &EBO[i]);
		}

		delete pshader;
	}

	inline void RenderScreen() override final
	{
		glfwPollEvents();//告诉GLFW检查所有等待处理的事件和消息，包括操作系统和窗口系统中应当处理的消息。包括鼠标、键盘的信息。
		//如果有消息正在等待，它会先处理这些消息再返回；否则该函数会立即返回。
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);//设置清楚屏幕时所要用的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清楚颜色和深度缓冲中所有颜色，相当于设置背景色
		this->pshader->UseProgram();		//开始用着色器


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textures[0]);
		this->pshader->SetInt("texture1", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->textures[1]);
		this->pshader->SetInt("texture2", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->textures[2]);
		this->pshader->SetInt("texture3", 2);


		do_movement();
		


		glm::mat4 model(1);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glm::mat4 view(1);
		view = pCamera->GetViewMatrix();
		glm::mat4 projection(1);
		projection = glm::perspective(glm::radians(pCamera->Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		
		this->pshader->SetInt("index", 0);
		this->pshader->SetMatrix4fv("model", model);
		this->pshader->SetMatrix4fv("view", view);
		this->pshader->SetMatrix4fv("projection", projection);

		glBindVertexArray(this->VAO[0]);	//绑定VAO
		glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); //解绑VAO

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(currentAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-2.2f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(currentAngleEarth), glm::vec3(0.0f, 0.0f, 1.0f));

		this->pshader->SetInt("index", 1);
		this->pshader->SetMatrix4fv("model", model);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glBindVertexArray(this->VAO[1]);	//绑定VAO
		glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); //解绑VAO

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(currentAngle), glm::vec3(0.1f, 0.2f, 0.9f));
		model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(currentAngleEarth), glm::vec3(0.0f, 1.0f, 1.0f));

		this->pshader->SetInt("index", 2);
		this->pshader->SetMatrix4fv("model", model);
		glBindVertexArray(this->VAO[2]);	//绑定VAO
		glDrawElements(GL_TRIANGLES,num, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); //解绑VAO


		glfwSwapBuffers(this->window);
		glfwPollEvents();
		
	}
	void initUI()
	{
		glfwSetKeyCallback(this->window, KeyCallback);
		glfwSetFramebufferSizeCallback(this->window, FramebufferSizeCallback);
		glfwSetCursorPosCallback(this->window, MouseCallback);
		//glfwSetMouseButtonCallback(this->window, mouse_button_callback);
		glfwSetScrollCallback(this->window, scroll_callback);
	}
	void CreateObjects()
	{
		this->CreateGalaxy();
	}
	void CreateShaders()
	{
		pshader = new Shader("coretwo.vs", "coretwo.fs");
	}
	bool IsWindowClosed()
	{
		return glfwWindowShouldClose(window);
	}

private:
	GLuint VAO[3];
	GLuint VBO[3];
	GLuint EBO[3];


	Shader* pshader;

	static bool keys[1024];
	static std::unique_ptr<CameraTwo> pCamera;

	unsigned int textures[3] = { 0 };

	int num;

	
	//地球自转角度
	float speedAngle = 100;
	float currentAngle = 0;
	//地球公转角度
	float speedAngleEarth = 10;
	float currentAngleEarth = 0;
	

	void CreateGalaxy()
	{
		vector<Point> points(2 + (vstepNum - 1) * (ustepNum + 1));
		vector<GLfloat> coords(2 * (2 + (vstepNum - 1) * (ustepNum + 1)));
		vector<int> indexes;

		double ustep = 1 / (double)ustepNum, vstep = 1 / (double)vstepNum;
		points[0] = getPoint(0, 0);
		coords[0] = 0.5;
		coords[1] = 1;
		int c = 1;
		for (int i = 1; i < vstepNum; i++)
		{
			for (int j = 0; j <= ustepNum; j++)
			{
				points[c] = getPoint(ustep * j, vstep * i);
				coords[2 * c] = 1 - ustep * j;
				coords[2 * c + 1] = vstep * i;
				c++;
			}
		}
		points[c] = getPoint(1, 1);
		coords[c++] = 0.5;
		coords[c] = 0;
		for (int i = 0; i <= ustepNum; i++)//球体上第一层
		{
			indexes.push_back(0);//上顶点总是第一个点
			indexes.push_back(1 + i);
			indexes.push_back(1 + (i + 1) % (ustepNum + 1));
		}
		//int a = ustepNum+1;
		for (int i = 1; i < vstepNum - 1; i++) {
			int start = 1 + (i - 1) * (ustepNum + 1);
			for (int j = start; j < start + (1 + ustepNum); j++) {

				/*
				*       j
				*       |\
				*       | \
				*       |__\
				* j+ustepn j + (ustepnum + 1) % vstepnum
				*/
				indexes.push_back(j);
				indexes.push_back(j + (1 + ustepNum));
				indexes.push_back(start + (1 + ustepNum) + (j + 1 - start) % (1 + ustepNum));
				/*
				*
				*         j ___ （j + 1）% ustepnum
				*           \  |
				*		     \ |
				*		      \|
				*     j + (ustepnum + 1) % vstepnum
				*/

				indexes.push_back(j);
				indexes.push_back(start + (j + 1 - start) % (1 + ustepNum));
				indexes.push_back(start + ustepNum + 1 + (j + 1 - start) % (1 + ustepNum));
			}
		}
		int last = 1 + (ustepNum + 1) * (vstepNum - 1);
		int start = 1 + (ustepNum + 1) * (vstepNum - 2);
		for (int i = 1 + (ustepNum + 1) * (vstepNum - 2); i < 1 + ustepNum * (vstepNum - 1); i++)//球体上最后一层
		{
			indexes.push_back(i);
			indexes.push_back(last); //逆时针排列
			indexes.push_back(start + ((1 + i) - start) % (1 + ustepNum));
		}
		this->num = indexes.size();
		float* vertices = new float[points.size() * 5];

		for (int i = 0; i < points.size(); i++) {
			vertices[5 * i] = points[i].x;
			vertices[5 * i + 1] = points[i].y;
			vertices[5 * i + 2] = points[i].z;

			vertices[5 * i + 3] = coords[2 * i];
			vertices[5 * i + 4] = coords[2 * i + 1];
			//cout << vertices[5 * i] << "  " << vertices[5 * i + 1] << " " << vertices[5 * i + 2] << endl;
		}
		unsigned int* indices = new unsigned int[indexes.size()];

		for (int i = 0; i < indexes.size(); i++)
		{
			indices[i] = indexes[i];
		}

		//最终顶点信息存在了vertices中，索引信息存在了indices中
		for (int i = 0; i < 3; i++) {
			glGenVertexArrays(1, &VAO[i]);
			glGenBuffers(1, &VBO[i]);
			glGenBuffers(1, &EBO[i]);
			glBindVertexArray(VAO[i]);

			glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		



		// load and create a texture 
		// -------------------------
		string names[5] = { "sun.jpg", "earth.jpg" ,"moon.jpg" };
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.


		for (int i = 0; i < 3; i++)
		{
			glGenTextures(1, &textures[i]);
			glBindTexture(GL_TEXTURE_2D, textures[i]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps
			// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
			unsigned char* data = stbi_load(string(names[i]).c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
		}
	}

	Point getPoint(double u, double v) {
		double x = sin(PI * v) * cos(2 * PI * u);
		double y = sin(PI * v) * sin(2 * PI * u);
		double z = cos(PI * v);
		return Point(x, y, z);
	}
	
	void do_movement()
	{
		// Camera controls
		GLfloat cameraSpeed = 5.0f * deltaTime;
		currentAngle = (currentAngle + speedAngle * deltaTime);
		currentAngleEarth += 10 * deltaTime;
		if (currentAngle > 360.0 + 1e-10)
		{
			currentAngle -= 360;
		}
		if (currentAngleEarth > 360.0 + 1E-10)
		{
			currentAngleEarth -= 360;
		}
		if (keys[GLFW_KEY_W])
			if (speedAngle < 0 / 3)
			{
				speedAngle += 0.01;
			}
		if (keys[GLFW_KEY_S])
			if (speedAngle > 5 / 3)
			{
				speedAngle -= 0.01;
			}
		if (keys[GLFW_KEY_A]) {
			if (speedAngleEarth < 40) {
				speedAngleEarth += 0.03;
			}
		}
		if (keys[GLFW_KEY_D]) {
			if (speedAngleEarth > 5) {
				speedAngleEarth -= 0.03;
			}
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
		//GLfloat xOffset, yOffset;
		//if (fisrtMouse)
		//{
		//	lastX = xPos;
		//	lastY = yPos;
		//	fisrtMouse = false;
		//}
		//xOffset = xPos - lastX;
		//yOffset = lastY - yPos;
		//lastX = xPos;
		//lastY = yPos;
		//pCamera->ProcessMouseMovement(xOffset, yOffset);
	}


	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		pCamera->ProcessMouseScroll(yoffset);
	}
};

std::unique_ptr<CameraTwo> homework2::pCamera = std::unique_ptr<CameraTwo>(new
	CameraTwo(glm::vec3(0.0f, -5.0f, 0.0f)));
bool homework2::keys[1024] = { false };





