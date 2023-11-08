#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;
using namespace glm;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//相机位置，控制移动
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static float fov = 45.0f; // 视野范围

//时间差
static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
static float lastFrame = 0.0f; // 上一帧的时间

static void _8_Camera()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLWindow", NULL, NULL);
	if (window == NULL)
	{
		cout << "GLFW Window Create Failed !!!" << endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//鼠标按键回调
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//鼠标位置回调
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	//鼠标滚轮回调
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// ========================= 

	Shader testShader("_7_3d");

	// ========================= 

	float vertices[] = {
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

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("icon2.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, NULL);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("logo.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, NULL);

	testShader.use();
	testShader.setI("texture1", 0);
	testShader.setI("texture2", 1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//OpenGL本身没有摄像机(Camera)的概念
	//可以通过把场景中的所有物体往相反方向移动的方式来模拟出摄像机，产生一种我们在移动的感觉，而不是场景在移动。

	//要定义一个摄像机，我们需要它在世界空间中的位置、观察的方向、一个指向它右侧的向量以及一个指向它上方的向量。

	//定义相机位置
	//	轴是从屏幕指向你的，如果希望摄像机向后移动，就沿着z轴的正方向移动。
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	//定义相机z方向
	//	注意：是指向屏幕外面的反方向
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	//定义相机x方向（只需考虑xz平面，取相对于xz平面的up向量叉乘）
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	//定义相机y方向
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	//使用3个相互垂直的轴定义了一个坐标空间，可以用这3个轴外加一个平移向量来创建一个矩阵
	//	用这个矩阵乘以任何向量来将其变换到那个坐标空间。这就是LookAt矩阵
	// 
	// LookAt 矩阵
	//	[ Rx Ry Rz 0 ] * [ 1 0 0 -Px ]
	//	[ Ux Uy Uz 0 ] * [ 0 1 0 -Py ]
	//	[ Dx Dy Dz 0 ] * [ 0 0 1 -Pz ]
	//	[ 0  0  0  1 ] * [ 0 0 0  1  ]
	//
	//	其中R是右向量，U是上向量，D是方向向量，P是摄像机位置向量

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.5f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		testShader.use();

		float time = (float)glfwGetTime();

		//记录时间差，用来算平均速度的
		//	有些人可能会比其他人每秒绘制更多帧
		//	deltaTime很大，就意味着上一帧的渲染花费了更多时间，所以这一帧的速度需要变得更高来平衡渲染所花去的时间
		//	保证摄像机的速度都会相应平衡，这样每个用户的体验就都一样了
		deltaTime = time - lastFrame;
		lastFrame = time;

		//让相机位置在xz轴10半径的圆旋转
		//float radius = 10.0f;
		//float camX = sin(time) * radius;
		//float camZ = cos(time) * radius;

		//glm 提供了lookAt接口，返回LookAt矩阵
		//	参数1：相机位置
		//	参数2：目标位置
		//	参数3：世界空间中向上的向量
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::lookAt(
		//	glm::vec3(glm::vec3(camX, 0.0, camZ)),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));

		//自定义Camera位置
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(
			glm::vec3(cameraPos),
			glm::vec3(cameraPos + cameraFront),
			glm::vec3(cameraUp));

		testShader.setM4("view", glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		testShader.setM4("projection", glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			testShader.setM4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	testShader.del();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//键盘WASD，负责控制相机位置移动
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

static bool enable_rotate;

//使用鼠标点击，负责控制是否启用旋转
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << button << "," << action << endl;

	if (action == GLFW_PRESS)
		enable_rotate = true;

	if (action == GLFW_RELEASE)
		enable_rotate = false;
}

//默认是屏幕的中心
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;

//俯仰角和偏航角
static float pitchValue = 0.0f;
static float yawValue = -90.0f;

//使用鼠标位置，负责控制相机旋转
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!enable_rotate)
	{
		lastX = xpos;
		lastY = ypos;
		return;
	}

	//欧拉角(Euler Angle)
	//	俯仰角(Pitch)、偏航角(Yaw)和滚转角(Roll)
	//	对应 X、Y、Z

	//对于摄像机系统，一般只关心俯仰角和偏航角，

	//1.在xz平面上，看向y轴（俯仰角）
	//	根据俯仰角求出方向向量的坐标
	//direction.x = cos(glm::radians(pitch));
	//direction.y = sin(glm::radians(pitch));
	//direction.z = cos(glm::radians(pitch));

	//2.在xz平面，忽略y轴（偏航角）
	//	根据偏航角求出方向向量的坐标
	//direction.x = cos(glm::radians(yaw));
	//direction.y = 0; 
	//direction.z = sin(glm::radians(yaw));

	//两个相叠加，就可以根据俯仰角+偏航角求出方向向量
	//direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	//direction.y = sin(glm::radians(pitch));
	//direction.z = cos(glm::radians(pitch)) 8 sin(glm::radians(yaw))

	//下面的问题就是：求出俯仰角+偏航角
	//根据鼠标的输入计算
	//	水平的移动影响偏航角，竖直的移动影响俯仰角
	//	储存上一帧鼠标的位置，在当前帧中我们当前计算鼠标位置与上一帧的位置相差多少

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意Y是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;

	//鼠标灵敏度
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	pitchValue += yoffset;
	yawValue += xoffset;

	//限制俯仰角（上下旋转范围）
	if (pitchValue > 89.0f) pitchValue = 89.0f;
	if (pitchValue < -89.0f) pitchValue = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitchValue)) * cos(glm::radians(yawValue));
	front.y = sin(glm::radians(pitchValue));
	front.z = cos(glm::radians(pitchValue)) * sin(glm::radians(yawValue));

	cameraFront = glm::normalize(front);
}

//使用鼠标滚轮，负责相机缩放 FOV
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 60.0f)
		fov -= yoffset;

	//限制FOV范围在，1-45之间
	if (fov <= 1.0f) fov = 1.0f;
	if (fov >= 60.0f) fov = 60.0f;
}