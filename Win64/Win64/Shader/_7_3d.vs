#version 330 core

layout (location=0) in vec3 aPos;		//0是顶点位置
layout (location=1) in vec2 aTexcoord;	//2是UV坐标

uniform mat4 model; //模型矩阵
uniform mat4 view; //观察矩阵
uniform mat4 projection; //投影矩阵

out vec2 Texcoord; //返回给片段着色器的UV

void main()
{
	//通过变换矩阵右乘
	vec4 pos = projection * view * model * vec4(aPos, 1.0f);

	//返回裁剪坐标
	gl_Position = pos;

	//返回UV
	Texcoord = aTexcoord;
}