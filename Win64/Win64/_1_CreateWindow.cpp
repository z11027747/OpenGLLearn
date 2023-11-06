
//##include "XXX"，在当前的源代码目录和编译器的搜索路径中搜索头文件
//##include <XXX>，在编译器的标准库路径中搜索头文件

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

static void _1_CreateWindow()
{
	cout << "Create Window" << endl;

	//初始化GLFW
	glfwInit();

	//glfwWindowHint 配置GLFW
	//	第一个参数是选项名称，一般是GLEW_XX
	//	第二个参数是选项值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //设置主版本是3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //设置小版本是3，也就是3.3版本

	//设置版本是3.3
	//	这样GLFW会在创建OpenGL上下文时做出适当的调整
	//	这也可以确保用户在没有适当的OpenGL版本支持的情况下无法运行

	//设置明确告诉GLFW我们使用的是核心模式(Core-profile)。
	//	明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集（没有我们已不再需要的向后兼容特性）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建Window，前两个参数是窗口的宽高，第三个是名称
	auto* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "GLFW Window Create Failed !!!" << endl;
		glfwTerminate();
		return;
	}

	//通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);

	//告诉OpenGL渲染窗口的尺寸大小，也就是视口大小
	//	这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标
	//glViewport(0, 0, 800, 600);

	//当用户改变窗口的大小的时候，视口也应该被调整
	//可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD是用来管理OpenGL的函数指针的
	// glfwGetProcAddress 是用来加载系统相关的OpenGL函数指针地址的函数，传入GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	//渲染循环
	//	主动关闭它之前不断绘制图像并能够接受用户输入
	while (!glfwWindowShouldClose(window)) //glfwWindowShouldClose 是否被要求退出（有相关输入）
	{
		//输入
		processInput(window);

		//渲染指令
		//每个新的渲染迭代开始的时候清屏，否则我们仍能看见上一次迭代的渲染结果
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//目前只关心颜色，分颜色缓冲、深度缓冲、模板缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		//OpenGL是面向状态的
		//	glClearColor 状态设置函数
		//	glClear	     状态使用函数

		//交换缓冲区：储存着GLFW窗口每一个像素颜色值的大缓冲
		//	应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
		//	这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的
		//	前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制
		//	当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来
		glfwSwapBuffers(window);

		//接受事件、更新状态
		glfwPollEvents();
	}

	//正确释放窗口资源
	glfwTerminate();
	return;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//...

	//重置视口大小
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
	//按下 Esc退出窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}