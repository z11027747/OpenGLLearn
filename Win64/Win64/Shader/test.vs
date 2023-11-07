#version 330 core

layout (location=0) in vec3 aPos; //0是顶点位置
layout (location=1) in vec3 aCol; //1是顶点颜色

uniform vec3 center; //自定义参数，代码传递

out vec3 outCol; //返回给片段着色器的颜色

void main()
{
	//根据传过来的center进行移动
	vec3 pos = aPos + (center)*0.5f;

	//翻转三角形
	pos.y *= -1;

	//返回裁剪坐标
	gl_Position = vec4(pos, 1.0f);

	//算距离当做mask影响颜色
	float dist = distance(pos, vec3(0.0f, 0.0f, 0.0f));
	outCol = aCol * dist;
}