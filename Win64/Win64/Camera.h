
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:

	//相机位置，控制移动
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//俯仰角和偏航角
	float Pitch;
	float Yaw;

	//视野范围FOV
	float Zoom;

	//相机移动速度和灵敏度
	float MovementSpeed;
	float MouseSensitivity;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//获取相机的观察矩阵
	glm::mat4 GetViewMatrix()
	{
		//定义相机位置
		//	轴是从屏幕指向你的，如果希望摄像机向后移动，就沿着z轴的正方向移动。
		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

		//定义相机z方向
		//	注意：是指向屏幕外面的反方向
		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

		//定义相机x方向（只需考虑xz平面，取相对于xz平面的up向量叉乘）
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		//定义相机y方向
		//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		//使用3个相互垂直的轴定义了一个坐标空间，可以用这3个轴外加一个平移向量来创建一个矩阵
		//	用这个矩阵乘以任何向量来将其变换到那个坐标空间。这就是LookAt矩阵
		// 
		// LookAt 矩阵
		//	[ Rx Ry Rz 0 ] * [ 1 0 0 -Px ]
		//	[ Ux Uy Uz 0 ] * [ 0 1 0 -Py ]
		//	[ Dx Dy Dz 0 ] * [ 0 0 1 -Pz ]
		//	[ 0  0  0  1 ] * [ 0 0 0  1  ]
		//
		//	其中R是右向量，U是上向量，D是方向向量，P是摄像机位置向量

		//让相机位置在xz轴10半径的圆旋转
		//float radius = 10.0f;
		//float camX = sin(time) * radius;
		//float camZ = cos(time) * radius;

		//glm 提供了lookAt接口，返回LookAt矩阵
		//	参数1：相机位置
		//	参数2：目标位置
		//	参数3：世界空间中向上的向量
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::lookAt(
		//	glm::vec3(glm::vec3(camX, 0.0, camZ)),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));

		return glm::lookAt(Position, Position + Front, Up);
	}

	//键盘WASD，负责控制相机位置移动
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	//使用鼠标位置，负责控制相机旋转
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			//限制俯仰角（上下旋转范围）
			Zoom = glm::clamp(Zoom, -89.0f, 89.0f);
		}

		updateCameraVectors();
	}

	//使用鼠标滚轮，负责相机缩放 FOV
	void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;

		//限制FOV范围在，1-60之间
		Zoom = glm::clamp(Zoom, 1.0f, 60.0f);
	}

private:

	//更新相机欧拉角向量
	void updateCameraVectors()
	{
		//欧拉角(Euler Angle)
		//	俯仰角(Pitch)、偏航角(Yaw)和滚转角(Roll)
		//	对应 X、Y、Z

		//对于摄像机系统，一般只关心俯仰角和偏航角，

		//1.在xz平面上，看向y轴（俯仰角）
		//	根据俯仰角求出方向向量的坐标
		//direction.x = cos(glm::radians(pitch));
		//direction.y = sin(glm::radians(pitch));
		//direction.z = cos(glm::radians(pitch));

		//2.在xz平面，忽略y轴（偏航角）
		//	根据偏航角求出方向向量的坐标
		//direction.x = cos(glm::radians(yaw));
		//direction.y = 0; 
		//direction.z = sin(glm::radians(yaw));

		//两个相叠加，就可以根据俯仰角+偏航角求出方向向量
		//direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		//direction.y = sin(glm::radians(pitch));
		//direction.z = cos(glm::radians(pitch)) 8 sin(glm::radians(yaw))

		//下面的问题就是：求出俯仰角+偏航角
		//根据鼠标的输入计算
		//	水平的移动影响偏航角，竖直的移动影响俯仰角
		//	储存上一帧鼠标的位置，在当前帧中我们当前计算鼠标位置与上一帧的位置相差多少

		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

};

#endif CAMERA_H

