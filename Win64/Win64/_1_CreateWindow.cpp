
//##include "XXX"���ڵ�ǰ��Դ����Ŀ¼�ͱ�����������·��������ͷ�ļ�
//##include <XXX>���ڱ������ı�׼��·��������ͷ�ļ�

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);

static void _1_CreateWindow()
{
	cout << "Create Window" << endl;

	//��ʼ��GLFW
	glfwInit();

	//glfwWindowHint ����GLFW
	//	��һ��������ѡ�����ƣ�һ����GLEW_XX
	//	�ڶ���������ѡ��ֵ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //�������汾��3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //����С�汾��3��Ҳ����3.3�汾

	//���ð汾��3.3
	//	����GLFW���ڴ���OpenGL������ʱ�����ʵ��ĵ���
	//	��Ҳ����ȷ���û���û���ʵ���OpenGL�汾֧�ֵ�������޷�����

	//������ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)��
	//	��ȷ����GLFW������Ҫʹ�ú���ģʽ��ζ������ֻ��ʹ��OpenGL���ܵ�һ���Ӽ���û�������Ѳ�����Ҫ�����������ԣ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//����Window��ǰ���������Ǵ��ڵĿ�ߣ�������������
	auto* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "GLFW Window Create Failed !!!" << endl;
		glfwTerminate();
		return;
	}

	//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);

	//����OpenGL��Ⱦ���ڵĳߴ��С��Ҳ�����ӿڴ�С
	//	����OpenGL��ֻ��֪���������ݴ��ڴ�С��ʾ���ݺ�����
	//glViewport(0, 0, 800, 600);

	//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
	//���ԶԴ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD����������OpenGL�ĺ���ָ���
	// glfwGetProcAddress ����������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���������GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	//��Ⱦѭ��
	//	�����ر���֮ǰ���ϻ���ͼ���ܹ������û�����
	while (!glfwWindowShouldClose(window)) //glfwWindowShouldClose �Ƿ�Ҫ���˳�����������룩
	{
		//����
		processInput(window);

		//��Ⱦָ��
		//ÿ���µ���Ⱦ������ʼ��ʱ�������������������ܿ�����һ�ε�������Ⱦ���
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//Ŀǰֻ������ɫ������ɫ���塢��Ȼ��塢ģ�建��
		glClear(GL_COLOR_BUFFER_BIT);

		//OpenGL������״̬��
		//	glClearColor ״̬���ú���
		//	glClear	     ״̬ʹ�ú���

		//������������������GLFW����ÿһ��������ɫֵ�Ĵ󻺳�
		//	Ӧ�ó���ʹ�õ������ͼʱ���ܻ����ͼ����˸�����⡣
		//	������Ϊ���ɵ�ͼ����һ���ӱ����Ƴ����ģ����ǰ��մ����ң����϶��������صػ��ƶ��ɵ�
		//	ǰ���屣�������������ͼ����������Ļ����ʾ�������еĵ���Ⱦָ����ں󻺳��ϻ���
		//	�����е���Ⱦָ��ִ����Ϻ����ǽ���(Swap)ǰ����ͺ󻺳壬����ͼ����������Գ���
		glfwSwapBuffers(window);

		//�����¼�������״̬
		glfwPollEvents();
	}

	//��ȷ�ͷŴ�����Դ
	glfwTerminate();
	return;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//...

	//�����ӿڴ�С
	glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow* window)
{
	//���� Esc�˳�����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}