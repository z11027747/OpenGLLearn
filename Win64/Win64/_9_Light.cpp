#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

using namespace std;
using namespace glm;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//加载贴图
static unsigned int loadTexture(char const* path);

//屏幕宽高
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

//自定义相机
static Camera camera(
	glm::vec3(0.0f, 0.0f, 5.0f), //pos
	glm::vec3(0.0f, 1.0f, 0.0f)	 //up
);

//时间差
static float deltaTime = 0.0f; // 当前帧与上一帧的时间差
static float lastFrame = 0.0f; // 上一帧的时间

static void _9_Light()
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

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// ========================= 

	Shader cubeShader("_9_Cube");
	Shader lightingShader("_9_Lighting");

	// ========================= 

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ========================= Cube

	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ========================= Lighting

	unsigned int lightingVAO;
	glGenVertexArrays(1, &lightingVAO);
	glBindVertexArray(lightingVAO);

	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");

	cubeShader.use();

	//material
	cubeShader.setI("material.diffuse", 0);
	cubeShader.setI("material.specular", 1);
	cubeShader.setF("material.shininess", 32.0f);

	// positions all containers
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

	//平行光位置
	glm::vec3 dirLightPos(5.0f, 5.0f, 5.0f);
	//平行光颜色
	glm::vec3 dirLightCol(0.1f);

	//点光源位置
	glm::vec3 pointLightPoses[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
	};
	//点光源颜色
	glm::vec3 pointLightCols[] = {
		glm::vec3(0.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.8f, 0.0f),
	};

	//聚光灯颜色
	glm::vec3 spotLightCol(0.8f);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		float time = (float)glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		cubeShader.use();

		//Base
		cubeShader.setM4("view", view);
		cubeShader.setM4("projection", projection);
		cubeShader.setF3("objectColor", 1.0f, 1.0f, 1.0f);
		cubeShader.setF3("cameraPos", camera.Position);

		//平行光
		glm::vec3 dirLightDir = glm::normalize(dirLightPos);

		cubeShader.setF3("dirLight.direction", dirLightDir);
		cubeShader.setF3("dirLight.ambient", dirLightCol * 0.1f);
		cubeShader.setF3("dirLight.diffuse", dirLightCol);
		cubeShader.setF3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		//点光源
		pointLightPoses[0] = glm::vec3(sin(time), 0.0f, cos(time)) * 3.0f;

		cubeShader.setF3("pointLights[0].position", pointLightPoses[0]);
		cubeShader.setF3("pointLights[0].ambient", pointLightCols[0] * 0.1f);
		cubeShader.setF3("pointLights[0].diffuse", pointLightCols[0]);
		cubeShader.setF3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		cubeShader.setF("pointLights[0].constant", 1.0f);
		cubeShader.setF("pointLights[0].linear", 0.09f);
		cubeShader.setF("pointLights[0].quadratic", 0.032f);

		pointLightPoses[1] = glm::vec3(0.0f, sin(time), cos(time)) * 3.0f;

		cubeShader.setF3("pointLights[1].position", pointLightPoses[1]);
		cubeShader.setF3("pointLights[1].ambient", pointLightCols[1] * 0.1f);
		cubeShader.setF3("pointLights[1].diffuse", pointLightCols[1]);
		cubeShader.setF3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		cubeShader.setF("pointLights[1].constant", 1.0f);
		cubeShader.setF("pointLights[1].linear", 0.09f);
		cubeShader.setF("pointLights[1].quadratic", 0.032f);

		//聚光灯（假定相机发射的）
		cubeShader.setF3("spotLight.position", camera.Position);
		cubeShader.setF3("spotLight.direction", camera.Front);
		cubeShader.setF3("spotLight.ambient", spotLightCol * 0.1f);
		cubeShader.setF3("spotLight.diffuse", spotLightCol);
		cubeShader.setF3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		cubeShader.setF("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
		cubeShader.setF("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			cubeShader.setM4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glm::mat4 dirLightingModel = glm::mat4(1.0f);
		//dirLightingModel = glm::translate(dirLightingModel, dirLightPos);
		//dirLightingModel = glm::scale(dirLightingModel, glm::vec3(0.3f));

		//lightingShader.use();
		//lightingShader.setM4("model", dirLightingModel);
		//lightingShader.setM4("view", view);
		//lightingShader.setM4("projection", projection);

		//glBindVertexArray(lightingVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(lightingVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			glm::mat4 pointLightModel = glm::mat4(1.0f);
			pointLightModel = glm::translate(pointLightModel, pointLightPoses[i]);
			pointLightModel = glm::scale(pointLightModel, glm::vec3(0.3f));

			lightingShader.use();
			lightingShader.setM4("model", pointLightModel);
			lightingShader.setM4("view", view);
			lightingShader.setM4("projection", projection);
			lightingShader.setF3("lightCol", pointLightCols[i]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cubeShader.del();
	lightingShader.del();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightingVAO);
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
}

static bool enable_rotate;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		enable_rotate = true;
	if (action == GLFW_RELEASE)
		enable_rotate = false;
}

static float lastX;
static float lastY;

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!enable_rotate)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		return;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // 注意Y是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

//加载贴图
static unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}