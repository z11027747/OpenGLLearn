#version 330 core

layout (location=0) in vec3 positionOS;
layout (location=1) in vec3 normalOS;

out vec3 positionWS;
out vec3 normalWS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(positionOS, 1.0f);

	//计算世界空间坐标，乘以模型矩阵
	positionWS = vec3(model * vec4(positionOS, 1.0));

	//计算法线世界方向
	//
	//不能直接乘以模型矩阵
	//1.法向量只是一个方向向量，不能表达空间中的特定位置
	//	对于法向量，没有齐次坐标（顶点位置中的w分量），只希望对它实施缩放和旋转变换。
	//2.模型矩阵执行了不等比缩放，顶点的改变会导致法向量不再垂直于表面了
	//
	//使用一个为法向量专门定制的模型矩阵。这个矩阵称之为法线矩阵(Normal Matrix)
	//	法线矩阵：模型矩阵左上角3x3部分的逆矩阵的转置矩阵

	//计算比较耗费性能，一般都提前算出来传进去
	//mat3 normalMatrix = mat3(transpose(inverse(model)));
	//normalWS = normalMatrix * normalOS;

	//对于一般模型 没有缩放的话 直接乘就行了
	normalWS = mat3(model) * normalOS;
}