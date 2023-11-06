#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);


static void _2_CreateTriangle()
{
	cout << "Create Triangle" << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	// ========================= 构建和编译Shader程序

	//1.顶点着色器处理数据
	//GLSL(OpenGL Shading Language)

	//顶点着色器代码
	const char* vertexShaderSource =
		//版本声明，对应3.3
		"#version 330 core\n"
		//layout (location = 0) 设定了输入变量的位置值
		//in关键字，在顶点着色器中声明所有的输入顶点属性，这里需要顶点位置
		"layout (location = 0) in vec3 aPos;\n"
		//main函数，跟c语音很像
		"void main()\n"
		"{\n"
		//gl_Position变量是置数据的预定义的变量，最后需要给这个赋值
		//顶点就直接定义在NDC空间了，所以直接赋值即可
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\0";

	//为了能够让OpenGL使用它，我们必须在运行时动态编译顶点着色器的源代码

	//创建一个着色器对象，ID用来引用
	unsigned int vertexShader;
	//使用glCreateShader创建这个着色器，返回ID
	//	GL_VERTEX_SHADER代表顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//这个着色器源码附加到着色器对象上
	//	参数1：着色器对象引用
	//	参数2：传递字符串的数量
	//	参数3：着色器源码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译Shader
	glCompileShader(vertexShader);

	//需要检测编译是否成功，并且错误是什么，才可以修复

	int  success; //是否成功
	char infoLog[512]; //错误信息容器

	//通过glCompileShader检测时候编译成功
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		//通过glGetShaderInfoLog获取错误信息进行打印
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//2.片段着色器处理数据

	//片段着色器代码
	const char* fragmentShaderSource =
		//版本声明，对应3.3
		"#version 330 core\n"
		//用out关键字，声明最终的输出颜色 FragColor
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		//返回颜色值，RGBA
		"   FragColor = vec4(1.0f, 0f, 0f, 1.0f);\n"
		"}\0";

	//创建片段着色器对象，类型使用 GL_FRAGMENT_SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//通过glGetShaderInfoLog获取错误信息进行打印
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//3.把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中

	//要使用编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序
	//已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
	//当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入
	//当输出和输入不匹配的时候，你会得到一个连接错误

	//glCreateProgram函数创建一个程序对象
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//把之前编译的着色器附加到程序对象上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//用glLinkProgram链接它们
	glLinkProgram(shaderProgram);

	//检测链接着色器程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//调用glUseProgram函数，用刚创建的程序对象作为它的参数，以激活这个程序对象
	// TODO 渲染对象时候调用即可
	//glUseProgram(shaderProgram);

	//激活之后每个着色器调用和渲染调用都会使用这个程序对象（也就是之前写的着色器)了

	//着色器对象链接到程序对象以后，删除着色器对象，不需要它们了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ========================= 设置顶点数据

	//1.定义顶点
	//	OpenGL是一个3D图形库，所以在OpenGL中我们指定的所有坐标都是3D坐标（x、y和z）
	//	将它顶点的z坐标设置为0.0，深度都是一样的，看起来是2D
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//2.把顶点数据发送给显卡
	//会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡

	//通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存（通常被称为显存）中储存大量顶点

	//使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次
	//从CPU把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据
	//当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程

	//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象：
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//3.链接顶点属性

	//顶点数据缓冲区 VBO		(v1_x v1_y v1_z|v2_x v2_y v2_z|...)
	//	位置数据被储存为32位（4字节）浮点值
	//	每个位置包含3个这样的值
	//	在这3个值之间没有空隙（或其他值）。这几个值在数组中紧密排列(Tightly Packed)
	//	数据中第一个值在缓冲开始的位置

	//顶点数组对象(Vertex Array Object, VAO)
	//	可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中
	//	当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了
	//否则每个对象都需要进行一系列操作：
	//	1.使用一个顶点缓冲对象将顶点数据初始化至缓冲中
	//	2.建立了一个顶点和一个片段着色器
	//	3.并告诉了OpenGL如何把顶点数据链接到顶点着色器的顶点属性上
	//如果对象顶点数据比较多，并且对象比较多，流程就比较复杂
	//所以希望优化一下，绑定VAO就可以了

	//VAO存储的内容：
	//	1.glEnableVertexAttribArray和glDisableVertexAttribArray的调用
	//	2.通过glVertexAttribPointer设置的顶点属性配置
	//	3.通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象

	//OpenGL的核心模式要求必须使用VAO

	//创建 VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//绑定 VAO
	glBindVertexArray(VAO);

	//顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	//使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
	//可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
	//	参数1：目标缓冲类型
	//	参数2：数据大小
	//	参数3：数据
	//	参数4：显卡如何管理数据	
	//			GL_STATIC_DRAW ：数据不会或几乎不会改变
	//			GL_DYNAMIC_DRAW：数据会被改变很多
	//			GL_STREAM_DRAW ：数据每次绘制时都会改变
	//				比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW
	//					这样就能确保显卡把数据放在能够高速写入的内存部分
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//目前问题：
	//OpenGL还不知道它该如何解释内存中的顶点数据，以及它该如何将顶点数据链接到顶点着色器的属性上

	//顶点着色器允许我们指定任何以顶点属性为形式的输入
	//这使其具有很强的灵活性的同时，它还的确意味着我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性

	//使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	//	参数1：要配置的顶点属性				对应=>layout(location = 0)
	//	参数2：顶点属性大小					vec3是三个值，所以是3
	//	参数3：指定数据类型					GL_FLOAT(GLSL中vec*都是由浮点数值组成的)
	//	参数4：数据是否标准化				如果是True，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
	//	参数5：连续的顶点属性组之间的间隔	由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)
	//	参数6：偏移量(Offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//以顶点属性位置值作为参数，启用顶点属性
	glEnableVertexAttribArray(0);

	//解除绑定 VBO
	//对glVertexAttribPointer的调用将VBO注册为顶点属性的绑定顶点缓冲区对象，这样之后就可以安全地解除绑定
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	//绘制完物体后，再解绑VAO
	glBindVertexArray(NULL);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//渲染一个物体时要使用着色器程序
		glUseProgram(shaderProgram);
		//打算绘制物体的时候就拿出相应的VAO，绑定它
		glBindVertexArray(VAO);

		//glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型
		//	参数1：GL_TRIANGLES 是绘制三角形
		//	参数2：指定了顶点数组的起始索引，这里是0
		//	参数3：最打算绘制多少个顶点，这里是3
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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