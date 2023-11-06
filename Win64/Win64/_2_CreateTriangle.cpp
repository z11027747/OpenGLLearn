
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

	// ========================= �����ͱ���Shader����

	//1.������ɫ����������
	//GLSL(OpenGL Shading Language)

	//������ɫ������
	const char* vertexShaderSource =
		//�汾��������Ӧ3.3
		"#version 330 core\n"
		//layout (location = 0) �趨�����������λ��ֵ
		//in�ؼ��֣��ڶ�����ɫ�����������е����붥�����ԣ�������Ҫ����λ��
		"layout (location = 0) in vec3 aPos;\n"
		//main��������c��������
		"void main()\n"
		"{\n"
		//gl_Position�����������ݵ�Ԥ����ı����������Ҫ�������ֵ
		//�����ֱ�Ӷ�����NDC�ռ��ˣ�����ֱ�Ӹ�ֵ����
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\0";

	//Ϊ���ܹ���OpenGLʹ���������Ǳ���������ʱ��̬���붥����ɫ����Դ����

	//����һ����ɫ������ID��������
	unsigned int vertexShader;
	//ʹ��glCreateShader���������ɫ��������ID
	//	GL_VERTEX_SHADER��������ɫ��
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//�����ɫ��Դ�븽�ӵ���ɫ��������
	//	����1����ɫ����������
	//	����2�������ַ���������
	//	����3����ɫ��Դ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//����Shader
	glCompileShader(vertexShader);

	//��Ҫ�������Ƿ�ɹ������Ҵ�����ʲô���ſ����޸�

	int  success; //�Ƿ�ɹ�
	char infoLog[512]; //������Ϣ����

	//ͨ��glCompileShader���ʱ�����ɹ�
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		//ͨ��glGetShaderInfoLog��ȡ������Ϣ���д�ӡ
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//2.Ƭ����ɫ����������

	//Ƭ����ɫ������
	const char* fragmentShaderSource =
		//�汾��������Ӧ3.3
		"#version 330 core\n"
		//��out�ؼ��֣��������յ������ɫ FragColor
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		//������ɫֵ��RGBA
		"   FragColor = vec4(1.0f, 0f, 0f, 1.0f);\n"
		"}\0";

	//����Ƭ����ɫ����������ʹ�� GL_FRAGMENT_SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//ͨ��glGetShaderInfoLog��ȡ������Ϣ���д�ӡ
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//3.��������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)��

	//Ҫʹ�ñ������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������Ȼ������Ⱦ�����ʱ�򼤻������ɫ������
	//�Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ��
	//��������ɫ����һ�������ʱ�������ÿ����ɫ����������ӵ��¸���ɫ��������
	//����������벻ƥ���ʱ�����õ�һ�����Ӵ���

	//glCreateProgram��������һ���������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//��֮ǰ�������ɫ�����ӵ����������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//��glLinkProgram��������
	glLinkProgram(shaderProgram);

	//���������ɫ�������Ƿ�ʧ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//����glUseProgram�������øմ����ĳ��������Ϊ���Ĳ������Լ�������������
	// TODO ��Ⱦ����ʱ����ü���
	//glUseProgram(shaderProgram);

	//����֮��ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������Ҳ����֮ǰд����ɫ��)��

	//��ɫ���������ӵ���������Ժ�ɾ����ɫ�����󣬲���Ҫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ========================= ���ö�������

	//1.���嶥��
	//	OpenGL��һ��3Dͼ�ο⣬������OpenGL������ָ�����������궼��3D���꣨x��y��z��
	//	���������z��������Ϊ0.0����ȶ���һ���ģ���������2D
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//2.�Ѷ������ݷ��͸��Կ�
	//����GPU�ϴ����ڴ����ڴ������ǵĶ������ݣ���Ҫ����OpenGL��ν�����Щ�ڴ棬����ָ������η��͸��Կ�

	//ͨ�����㻺�����(Vertex Buffer Objects, VBO)��������ڴ棬������GPU�ڴ棨ͨ������Ϊ�Դ棩�д����������

	//ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ��
	//��CPU�����ݷ��͵��Կ���Խ���������ֻҪ�������Ƕ�Ҫ���Ծ���һ���Է��;����ܶ������
	//�����ݷ������Կ����ڴ��к󣬶�����ɫ���������������ʶ��㣬���Ǹ��ǳ���Ĺ���

	//ʹ��glGenBuffers������һ������ID����һ��VBO����
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//3.���Ӷ�������

	//�������ݻ����� VBO		(v1_x v1_y v1_z|v2_x v2_y v2_z|...)
	//	λ�����ݱ�����Ϊ32λ��4�ֽڣ�����ֵ
	//	ÿ��λ�ð���3��������ֵ
	//	����3��ֵ֮��û�п�϶��������ֵ�����⼸��ֵ�������н�������(Tightly Packed)
	//	�����е�һ��ֵ�ڻ��忪ʼ��λ��

	//�����������(Vertex Array Object, VAO)
	//	�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO��
	//	�����ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO������
	//����ÿ��������Ҫ����һϵ�в�����
	//	1.ʹ��һ�����㻺����󽫶������ݳ�ʼ����������
	//	2.������һ�������һ��Ƭ����ɫ��
	//	3.��������OpenGL��ΰѶ����������ӵ�������ɫ���Ķ���������
	//������󶥵����ݱȽ϶࣬���Ҷ���Ƚ϶࣬���̾ͱȽϸ���
	//����ϣ���Ż�һ�£���VAO�Ϳ�����

	//VAO�洢�����ݣ�
	//	1.glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ���
	//	2.ͨ��glVertexAttribPointer���õĶ�����������
	//	3.ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����

	//OpenGL�ĺ���ģʽҪ�����ʹ��VAO

	//���� VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//�� VAO
	glBindVertexArray(VAO);

	//���㻺�����Ļ���������GL_ARRAY_BUFFER
	//ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)
	//���Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	//	����1��Ŀ�껺������
	//	����2�����ݴ�С
	//	����3������
	//	����4���Կ���ι�������	
	//			GL_STATIC_DRAW �����ݲ���򼸺�����ı�
	//			GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶ�
	//			GL_STREAM_DRAW ������ÿ�λ���ʱ����ı�
	//				����˵һ�������е����ݽ�Ƶ�����ı䣬��ôʹ�õ����;���GL_DYNAMIC_DRAW��GL_STREAM_DRAW
	//					��������ȷ���Կ������ݷ����ܹ�����д����ڴ沿��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Ŀǰ���⣺
	//OpenGL����֪��������ν����ڴ��еĶ������ݣ��Լ�������ν������������ӵ�������ɫ����������

	//������ɫ����������ָ���κ��Զ�������Ϊ��ʽ������
	//��ʹ����к�ǿ������Ե�ͬʱ��������ȷ��ζ�����Ǳ����ֶ�ָ���������ݵ���һ�����ֶ�Ӧ������ɫ������һ����������

	//ʹ��glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
	//	����1��Ҫ���õĶ�������				��Ӧ=>layout(location = 0)
	//	����2���������Դ�С					vec3������ֵ��������3
	//	����3��ָ����������					GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)
	//	����4�������Ƿ��׼��				�����True���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮��
	//	����5�������Ķ���������֮��ļ��	�����¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)
	//	����6��ƫ����(Offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//�Զ�������λ��ֵ��Ϊ���������ö�������
	glEnableVertexAttribArray(0);

	//����� VBO
	//��glVertexAttribPointer�ĵ��ý�VBOע��Ϊ�������Եİ󶨶��㻺������������֮��Ϳ��԰�ȫ�ؽ����
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	//������������ٽ��VAO
	glBindVertexArray(NULL);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��Ⱦһ������ʱҪʹ����ɫ������
		glUseProgram(shaderProgram);
		//������������ʱ����ó���Ӧ��VAO������
		glBindVertexArray(VAO);

		//glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ������
		//	����1��GL_TRIANGLES �ǻ���������
		//	����2��ָ���˶����������ʼ������������0
		//	����3���������ƶ��ٸ����㣬������3
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