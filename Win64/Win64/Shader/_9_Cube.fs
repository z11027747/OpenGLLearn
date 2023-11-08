#version 330 core

in vec3 positionWS;
in vec3 normalWS;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    //光的一个属性是，它可以向很多方向发散并反弹，从而能够到达不是非常直接临近的点
    //光能够在其它的表面上反射，对一个物体产生间接的影响
    //全局照明(Global Illumination)算法，但是这种算法既开销高昂又极其复杂。
    //
    //简化版本：环境光照
    //  使用一个很小的常量（光照）颜色，添加到物体片段的最终颜色中

    //环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //漫反射
    vec3 lightDir = normalize(lightPos - positionWS);
    vec3 normalDir = normalize(normalWS);
    vec3 diffuse = max(0.0, dot(normalDir, lightDir)) * lightColor;

    //高光反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - positionWS);
    vec3 halfDir = normalize(viewDir + lightDir);
    vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), 10.0) * specularStrength * lightColor;

    vec3 lightingResult = ambient + diffuse + specular;

    FragColor = vec4(lightingResult * objectColor, 1.0);
}