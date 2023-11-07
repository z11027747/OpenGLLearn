#version 330 core

in vec3 outCol; //接受顶点着色器返回的颜色值

out vec4 FragColor; //输出最后的颜色

void main()
{
    FragColor = vec4(outCol.rgb, 1.0f);
}