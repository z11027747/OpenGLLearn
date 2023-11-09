#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//stb_image.h是Sean Barrett的一个非常流行的单头文件图像加载库，它能够加载大部分流行的文件格式，
//通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码
//相当于让其变成cpp了

void test() {

	//使用stbi_load加载图片数据
	//	返回图像的宽度、高度、通道个数
	int width, height, nrChannels;
	unsigned char* data = stbi_load("test.png", &width, &height, &nrChannels, 0);

}