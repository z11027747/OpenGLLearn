#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace glm;

static void glm_test() {


	//1 0 0 0 <translate> 1	= 1 0 0 1
	//0 1 0 0 <translate> 1 = 0 1 0 1
	//0 0 1 0 <translate> 0 = 0 0 1 0
	//0 0 0 1 <translate> 0	= 0 0 0 1
	mat4 trans;

	//初始化方式1
	// trans = mat4(1.0f);

	//mat4中4行4列的元素是按照列的方式排列的
	//	主要是因为opengl在显存中是按列存储的，所以在准备好数据后可以直接将mat4复制到显存中
	//可以使用[]操作符来访问mat4的第几列，并使用二级[]来访问某一列的第几个元素，

	//初始化方式2，熟悉内存分布
	trans[0][0] = 1.0f; trans[1][0] = 0.0f; trans[2][0] = 0.0f; trans[3][0] = 0.0f;
	trans[0][1] = 0.0f; trans[1][1] = 1.0f; trans[2][1] = 0.0f; trans[3][1] = 0.0f;
	trans[0][2] = 0.0f; trans[1][2] = 0.0f; trans[2][2] = 1.0f; trans[3][2] = 0.0f;
	trans[0][3] = 0.0f; trans[1][3] = 0.0f; trans[2][3] = 0.0f; trans[3][3] = 1.0f;

	//缩放 0.5倍
	//注意：如果有旋转，应该先乘旋转矩阵，否则位移也会受到影响

	//0.5 0.0 0.0 0.0 * 1 = 0.5
	//0.0 0.5 0.0 0.0 * 0 = 0
	//0.0 0.0 0.5 0.0 * 0 = 0
	//0.0 0.0 0.0 1.0 * 1 = 1
	trans = scale(trans, vec3(0.5f, 0.5f, 0.5f));

	//旋转 90度
	trans = rotate(trans, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));

	//平移 1,1,0

	//0.5 0.0 0.0 1.0 * 1 = 1.5
	//0.0 0.5 0.0 1.0 * 0 = 1
	//0.0 0.0 0.5 0.0 * 0 = 0
	//0.0 0.0 0.0 1.0 * 1 = 1
	trans = translate(trans, vec3(1.0f, 1.0f, 0.0f));

	vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //w是1是坐标，代表位移是有效的

	vec = vec * trans;
	std::cout << vec.x << "," << vec.y << "," << vec.z << std::endl;
}