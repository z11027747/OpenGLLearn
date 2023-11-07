#version 330 core

layout (location=0) in vec3 aPos;		//0是顶点位置
layout (location=1) in vec3 aCol;		//1是顶点颜色
layout (location=2) in vec2 aTexcoord;	//2是UV坐标

uniform float time; //自定义参数，代码传递
uniform mat4 trans; //变换矩阵

out vec3 outCol; //返回给片段着色器的颜色
out vec2 Texcoord; //返回给片段着色器的颜色

void main()
{
	//通过变换矩阵右乘
	vec4 pos = trans * vec4(aPos, 1.0f);

	//返回裁剪坐标
	gl_Position = pos;

	//算距离当做mask影响颜色
	float dist = distance(pos.xyz, vec3(0.0f, 0.0f, 0.0f));
	outCol = aCol * dist;

	//返回UV
	Texcoord = aTexcoord;
}