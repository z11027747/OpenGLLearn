#ifndef SHADER
#define SHADER

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stb_image.h>

class Shader
{
public:
	//程序ID
	unsigned int ID;

	//构造函数，构建着色器
	Shader(const char* path);

	//使用/激活程序
	void use();
	void del();

	//uniform工具函数
	void setB(const std::string &name, bool v1) const;
	void setI(const std::string& name, int v1) const;
	void setF(const std::string& name, float v1) const;
	void setF3(const std::string& name, float v1, float v2, float v3) const;
	void setM4(const std::string& name, const float* value) const;
};

#endif SHADER

