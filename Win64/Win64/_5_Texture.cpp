#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "Shader.h"

using namespace std;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

static void _5_Texture()
{
	cout << "Create Uniform" << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "GLFW Window Create Failed !!!" << endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// ========================= 

	Shader testShader("_5_texture");

	// ========================= 

	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//定义纹理1
	unsigned int texture1;
	//通过glGenTextures生成纹理
	//	参数1：生成纹理的数量
	//	参数2：返回的纹理ID（多个就是数组）
	glGenTextures(1, &texture1);

	//通过Id绑定纹理，之后都操作这个纹理
	glBindTexture(GL_TEXTURE_2D, texture1);

	//设置环绕、过滤方式
	//	参数1：纹理目标 GL_TEXTURE_2D		就是上面绑定的
	//	参数2：类型
	//	参数3：值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//环绕（Wrap）：就是采样到边界的处理
	//	需要对XY分别设置，也就是：GL_TEXTURE_WRAP_S（X）和 GL_TEXTURE_WRAP_T（Y）
	//环绕方式有4种：
	//	1.GL_REPEAT	对纹理的默认行为。重复纹理图像
	//	2.GL_MIRRORED_REPEAT	和GL_REPEAT一样，但每次重复图片是镜像放置的
	//	3.GL_CLAMP_TO_EDGE	纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果
	//	4.GL_CLAMP_TO_BORDER	超出的坐标为用户指定的边缘颜色

	//过滤（Filter）：就是采样的取色方式
	//	纹理坐标不依赖于分辨率(Resolution)是任意浮点值，所以OpenGL需要知道怎样将纹理像素映射到纹理坐标
	//过滤有2种比较重要的：
	//	1.GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。
	//		就取最靠近的那个像素颜色
	//	2.GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色。
	//		附近的4像素颜色混合值

	//GL_TEXTURE_MIN_FILTER 和 GL_TEXTURE_MAG_FILTER 代表纹理被放大或者缩小时的采样方式
	//	一般被缩小时，都会使用 多级渐远纹理Mipmap，否则会出现细节丢失和内存浪费
	
	//多级渐远纹理Mipmap：它简单来说就是一系列的纹理图像，后一个纹理图像是前一个的二分之一
	//	OpenGL会使用不同的多级渐远纹理，即最适合物体的距离的那个

	//使用stb_image加载图片数据
	//	返回图像的宽度、高度、通道个数
	int width, height, nrChannels;
	unsigned char* data = stbi_load("icon2.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		//通过图片数据载入纹理
		//	参数1：纹理目标 GL_TEXTURE_2D		就是上面绑定的
		//	参数2：Mipmap的级别
		//	参数3：纹理格式		这里就是RGB
		//	参数4：纹理的宽度
		//	参数5：纹理的高度
		//	参数6：没有意义，就写0
		//	参数7：原图格式		RGB格式
		//	参数8：数据格式		使用二进制加载的
		//	参数9：数据实际内容
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//自动生成所有需要的多级渐远纹理 Mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		//生成了纹理和相应的多级渐远纹理后，就可以释放源数据了
		stbi_image_free(data);

		//解除绑定纹理
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//定义纹理2
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

	//解除绑定纹理
	glBindTexture(GL_TEXTURE_2D, NULL);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//纹理怎么传到片段着色器？
		// 
		//纹理单元(Texture Unit)：一个纹理的位置值通常称为一个纹理单元
		//	使用glUniform1i，可以给纹理采样器分配一个位置值，可以在一个片段着色器中设置多个纹理
		// 
		//通过把纹理单元赋值给采样器，我们可以一次绑定多个纹理
		// 
		// 在绑定纹理之前先激活纹理单元
		glActiveTexture(GL_TEXTURE0);

		//激活纹理单元之后，接下来的glBindTexture函数调用会绑定这个纹理到当前激活的纹理单元
		//	纹理单元GL_TEXTURE0默认总是被激活，不写也行
		//	OpenGL至少保证有16个纹理单元可使用 GL_TEXTURE0 - GL_TEXTRUE15
		glBindTexture(GL_TEXTURE_2D, texture1);

		//第二个纹理
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		testShader.use();

		//设置贴图，通过glUniform1i指定纹理单元
		testShader.setI("texture1", 0);
		testShader.setI("texture2", 1);

		float timeValue = (float)glfwGetTime();
		testShader.setF("time", timeValue);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	testShader.del();

	//释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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
}