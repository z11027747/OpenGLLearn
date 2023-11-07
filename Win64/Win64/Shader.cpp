#include "Shader.h"

Shader::Shader(const char* path)
{
	//1.从文件路径中获取着色器

	//源码定义
	std::string vertexCode;
	std::string fragmentCode;
	//ifstream流定义
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open("Shader/"+ std::string(path) + ".vs");
		fShaderFile.open("Shader/" + std::string(path) + ".fs");
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//2.编译着色器

	//顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//链接
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::del()
{
	glDeleteProgram(ID);
}

void Shader::setB(const std::string& name, bool v1) const
{
	//glGetUniformLocation 查询函数提供着色器程序和uniform的名字
	//	如果返回-1 则代表没找到
	auto location = glGetUniformLocation(ID, name.c_str());

	//glUniform1i设置uniform值（1i就是1个int）
	//  更新一个uniform之前必须先use，在当前激活的着色器程序中设置uniform的
	glUniform1i(location, (int)v1);
}

void Shader::setI(const std::string& name, int v1) const
{
	auto location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, v1);
}

void Shader::setF(const std::string& name, float v1) const
{
	auto location = glGetUniformLocation(ID, name.c_str());
	glUniform1f(location, v1);
}

void Shader::setF3(const std::string& name, float v1, float v2, float v3) const
{
	auto location = glGetUniformLocation(ID, name.c_str());
	glUniform3f(location, v1, v2, v3);
}