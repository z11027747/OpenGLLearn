#version 330 core

in vec3 outCol; //接受顶点着色器返回的颜色值
in vec2 Texcoord; //接受顶点着色器返回的UV

out vec4 FragColor; //输出最后的颜色

uniform sampler2D texture1; //定义采样器(Sampler)
uniform sampler2D texture2; //定义采样器(Sampler)

void main()
{
    //使用UV采样纹理
    vec4 tex1Col = texture(texture1, Texcoord);
    vec4 tex2Col = texture(texture2, Texcoord);

    FragColor = (tex1Col + tex2Col) * vec4(outCol.rgb, 1.0f);
}