#ifndef MODEL_H
#define MODEL_H

#include<iostream>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp> //Assimp相关的头文件
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

using namespace std;

//从文件加载纹理
static unsigned int TextureFromFile(const char* path);

//模型（Model）完整地表示一个模型
//定义一个模型类
//	作用：通过Assimp加载和转换模型文件，保存为 Mesh对象
class Model {
public:

	//构造函数，根据路径加载模型文件
	Model(const char* path) {
		loadModel(path);
	}

	//绘制所有网格数据
	void Draw(Shader shader) {
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}

	void del() {
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].del();
		}
	}

private:
	//模型中的网格数据
	vector<Mesh> meshes;

	//已经加载过的纹理缓存
	vector<Texture> textures_loaded;

	string directory;

	//加载模型
	void loadModel(string path) {

		Assimp::Importer importer;

		//加载模型至Assimp的一个叫做scene的数据结构中
		//	参数1：文件路径（各种文件类型都可以，内部封装好了）
		//	参数2：后处理选项
		//		aiProcess_Triangulate是转变三角形图元
		//		aiProcess_FlipUVs是翻转Y轴纹理坐标，OpenGL中要求0坐标在图片底部
		//		aiProcess_GenNormals是自动生成法线数据，如果没有的话
		//		aiProcess_OptimizeMeshes拼接成大网格，合并
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		//先会检查场景和其根节点不为null
		//然后检查了它的一个标记(Flag)，来查看返回的数据是不是不完整的
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		//根节点
		//	可能有一些父子关系的节点，取决于模型咋么做，所以递归处理
		processNode(scene->mRootNode, scene);
	}

	//按节点处理
	void processNode(aiNode* node, const aiScene* scene) {
		// 处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// 接下来对它的子节点重复这一过程
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	//处理单个Mesh
	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// 处理顶点位置、法线和纹理坐标
			Vertex vertex;

			//坐标
			glm::vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.Position = position;

			//法线
			if (mesh->HasNormals())
			{
				glm::vec3 normal;
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.Normal = normal;
			}

			//纹理坐标
			if (mesh->mTextureCoords[0])
			{
				glm::vec3 texcoord;
				texcoord.x = mesh->mTextureCoords[0][i].x;
				texcoord.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoord = texcoord;
			}
			else
				vertex.TexCoord = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		// 处理索引
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			//Assimp的接口定义了每个网格都有一个面(Face)数组
			//Face就是：面（三角形）
			//一个面包含了多个索引，索引顺序也是按渲染顺序的，直接加入即可
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 处理材质
		if (mesh->mMaterialIndex >= 0)
		{
			//Material 存在scene节点里，通过索引找
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			//需要找到材质里面的贴图
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	//从材质里面找贴图
	//	遍历了给定纹理类型的所有纹理位置，获取了纹理的文件位置
	//	并加载并和生成了纹理，将信息储存在了一个Vertex结构体中
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			//GetTexture 获取每个纹理的文件位置
			aiString str;
			mat->GetTexture(type, i, &str);

			//先检测是否加载过
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				Texture texture = textures_loaded[j];
				if (std::strcmp(texture.path.data(), str.C_Str()) == 0)
				{
					textures.push_back(texture);
					skip = true;
					break;
				}
			}

			//没加载则加载
			if (!skip)
			{
				Texture texture;
				string texturePath = directory + '/' + str.C_Str();
				texture.id = TextureFromFile(texturePath.c_str());
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
			}
		}
		return textures;
	}
};

static unsigned int TextureFromFile(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

#endif // !MODEL_H
