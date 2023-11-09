#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"

using namespace std;

//网格(Mesh)代表的是单个的可绘制实体
//定义一个网格类

//网格数据都有什么？
//1.顶点（位置向量、法向量、纹理坐标向量）
//2.用于索引绘制的索引

struct Vertex {
    glm::vec3 Position;     //位置
    glm::vec3 Normal;       //法线
    glm::vec2 TexCoord;    //纹理坐标
};

//3.纹理形式的材质数据（漫反射/镜面光贴图）
struct Texture {
    unsigned int id;    //纹理单元Id
    string type;        //类型

    //缓存路径，已经加载过就不加载了
    string path;
};

class Mesh {
public:

    //网格数据
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    //构造函数
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        //Vertex vertex;
        //vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);

        //vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f );  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f );  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f );  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f );  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f );  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f );  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);

        //vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f); -vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f); -vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f); -vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f); -vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f); -vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f); -vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);

        //vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);

        //vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, -0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);

        //vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 1.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  vertex.TexCoord = glm::vec2(1.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, 0.5f, 0.5f);  vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        //this->vertices.push_back(vertex);
        //vertex.Position = glm::vec3(-0.5f, 0.5f, -0.5f);  vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  vertex.TexCoord = glm::vec2(0.0f, 1.0);
        //this->vertices.push_back(vertex);

        setup();
    }

    //绘制，通过着色器
    // 
    //问题：
    //并不知道这个网格（如果有的话）有多少纹理、纹理是什么类型的
    //需要制定一系列规则
    //  每个漫反射纹理被命名为texture_diffuseN
    //  每个镜面光纹理应该被命名为texture_specularN

    //shader内按规则定义 uniform 采样器，如果不用就不可定义
    //  （虽然定义少的话会有点浪费绑定和uniform调用）
    void Draw(Shader shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(i);

            // 获取纹理序号（diffuse_textureN 中的 N）
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setI(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // 绘制网格
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(NULL);
    }

    void del() {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
    }

private:
    unsigned int VBO, VAO, EBO;

    //初始化缓冲Object
    void setup() {

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);

        //C++结构体的特性：内存布局是连续的(Sequential)
        //vector 本质就是数组，所以传过去的顶点数据也跟之前一样
        //size有变化：vertices.size() * sizeof(Vertex)

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //顶点属性是否激活 随着网格数据走，shader里面可以定义也可以不定义

        //步长：变成一个Vertex的结构
        //偏移：通过 offsetof 预处理指令计算

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
        glEnableVertexAttribArray(2);

        glBindVertexArray(NULL);
    }

};

#endif

