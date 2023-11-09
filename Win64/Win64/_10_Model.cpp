#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/anim.h>

#include <iostream>
#include <cmath>
#include <filesystem>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

using namespace std;
using namespace glm;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

// ========================= 
// 
// 无法实现复杂形状手工定义所有的顶点、法线和纹理坐标
// 
// 所以有个概念：模型（3D Model）
//	模型通常都由3D艺术家在Blender、3DS Max或者Maya这样的工具中精心制作
// 
// 3D建模工具(3D Modeling Tool)可以让艺术家创建复杂的形状，并使用一种叫做UV映射(uv-mapping)的手段来应用贴图
// 这些工具将会在导出到模型文件的时候自动生成所有的顶点坐标、顶点法线以及纹理坐标
// 
// 所有的技术细节都隐藏在了导出的模型文件中。
// 我们的工作就是解析这些导出的模型文件以及提取所有有用的信息，将它们储存为OpenGL能够理解的格式
//
// 但是模型的文件格式有很多种，每一种都会以它们自己的方式来导出模型数据
// 比如：
//	Wavefront的.obj这样的模型格式，只包含了模型数据以及材质信息，像是模型颜色和漫反射/镜面光贴图
//	wiki：https://en.wikipedia.org/wiki/Wavefront_.obj_file
// 
// 
//使用模型加载库：Assimp （本质就是解析文件数据，也可以自己写）
//	https://assimp.org/
//	Assimp能够导入很多种不同的模型文件格式，它会将所有的模型数据加载至Assimp的通用数据结构中
//	当Assimp加载完模型之后，我们就能够从Assimp的数据结构中提取我们所需的所有数据了
//	不论导入的是什么种类的文件格式，它都能够将我们从这些不同的文件格式中抽象出来，用同一种方式访问我们需要的数据
// 
//	当使用Assimp导入一个模型的时候，它通常会将整个模型加载进一个场景(Scene)对象，它会包含导入的模型/场景中的所有数据
// 
//	Scene包括一些子节点，Mesh和Material的数组
//	每个子节点都有自己的Mesh和Material的索引，去Scene数组里面查找
// 
// ========================= 

static void _10_Model()
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

	Shader cubeShader("_10_Model");

	Model customModel("D:/G/OpenGLLearn/Win64/Win64/Fox/Model/Fox.FBX");
	//Model customModel("D:/G/OpenGLLearn/Win64/Win64/nanosuit/nanosuit.obj");
	//Model customModel("D:/G/OpenGLLearn/Win64/Win64/plan.FBX");

	//平行光
	glm::vec3 dirLightDir = glm::vec3(1.0f);
	glm::vec3 dirLightCol(0.1f);

	cubeShader.use();
	cubeShader.setF3("objectColor", 1.0f, 1.0f, 1.0f);
	cubeShader.setF3("dirLight.direction", dirLightDir);
	cubeShader.setF3("dirLight.ambient", dirLightCol * 0.1f);
	cubeShader.setF3("dirLight.diffuse", dirLightCol);
	cubeShader.setF3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = (float)glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.02f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		cubeShader.use();
		cubeShader.setM4("model", model);
		cubeShader.setM4("view", view);
		cubeShader.setM4("projection", projection);

		//Draw model
		customModel.Draw(cubeShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cubeShader.del();
	customModel.del();

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