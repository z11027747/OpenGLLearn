#version 330 core

in vec3 positionWS;
in vec3 normalWS;
in vec2 uv;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 cameraPos;

struct Material {
    sampler2D diffuse;  //漫反射贴图
    sampler2D specular; //高光反射贴图
    float shininess;    //高光的半径
}; 

uniform Material material;


//平行光：太阳
//当一个光源处于很远的地方时，来自光源的每条光线就会近似于互相平行
//因为所有的光线都是平行的，所以物体与光源的相对位置是不重要的
//我们可以定义一个光线方向向量而不是位置向量来模拟一个定向光
struct DirLight {
    vec3 direction; //方向

    vec3 ambient;   //ambient强度
    vec3 diffuse;   //diffuse强度
    vec3 specular;  //specular强度
};
uniform DirLight dirLight;

//计算平行光颜色
vec3 CalcDirLight(DirLight light)
{
    vec3 diffuseCol = texture(material.diffuse, uv).rgb;
    vec3 specularCol = texture(material.specular, uv).rgb;

    //环境光
    vec3 ambient = light.ambient * diffuseCol;

    //漫反射
    vec3 lightDir = (light.direction);
    vec3 normalDir = normalize(normalWS);
    vec3 diffuse = max(0.0, dot(normalDir, lightDir)) * light.diffuse * diffuseCol;

    //高光反射
    vec3 viewDir = normalize(cameraPos - positionWS);
    vec3 halfDir = normalize(viewDir + lightDir);
    vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), material.shininess) * light.specular * specularCol;

    vec3 result = ambient + diffuse + specular;
    return result;
}

//点光源：灯泡
//分散在场景中的点，会朝着所有方向发光，会随着距离逐渐衰减
//
//衰减：随着光线传播距离的增长逐渐削减光的强度通常叫做衰减(Attenuation)
//  直接套公式
struct PointLight{
    vec3 position; //位置

    vec3 ambient;   //ambient强度
    vec3 diffuse;   //diffuse强度
    vec3 specular;  //specular强度

    float constant; //衰减公示常数项
    float linear;   //衰减公式一次项
    float quadratic;//衰减公式二次项

    //一般根据希望覆盖的距离设置，可以查表：
    //  http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
};
//一个uniform值数组。只是不能动态分配内存，必须 提前定义
//预处理指令来定义了我们场景中点光源的数量
#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

//计算点光源颜色
vec3 CalcPointLight(PointLight light)
{
    //计算和光源的距离
    float distance = length(light.position - positionWS);

    //衰减公式
    float attenuation = 1.0  / 
                            ( light.constant 
                            + light.linear * distance 
                            + light.quadratic * (distance * distance) );

    vec3 diffuseCol = texture(material.diffuse, uv).rgb;
    vec3 specularCol = texture(material.specular, uv).rgb;

    //环境光
    vec3 ambient = light.ambient * diffuseCol;

    //漫反射
    vec3 lightDir = normalize(light.position - positionWS);
    vec3 normalDir = normalize(normalWS);
    vec3 diffuse = max(0.0, dot(normalDir, lightDir)) * light.diffuse * diffuseCol;

    //高光反射
    vec3 viewDir = normalize(cameraPos - positionWS);
    vec3 halfDir = normalize(viewDir + lightDir);
    vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), material.shininess) * light.specular * specularCol;

    vec3 result = (ambient + diffuse + specular)*attenuation;
    return result;
}

//聚光灯：手电筒
//  位于环境中某个位置的光源，它只朝一个特定方向而不是所有方向照射光线
struct SpotLight {
    vec3  position;
    vec3  direction;  //方向

    vec3 ambient;   //ambient强度
    vec3 diffuse;   //diffuse强度
    vec3 specular;  //specular强度

    float cutOff;     //半径的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
    float outerCutOff;//更大的切光角，用来做渐变
};
uniform SpotLight spotLight;

//计算聚光灯颜色
vec3 CalcSpotLight(SpotLight light)
{
    //指向聚光灯的方向
    vec3 lightDir = normalize(light.position - positionWS);

    //和光源方向做点乘（取反保证方向一致）
    float theta = dot(-lightDir, normalize(light.direction));
    
    //余弦值比较为了性能
    //余弦值越接近1.0，它的角度就越小
    if (theta > light.cutOff)
    {
        vec3 diffuseCol = texture(material.diffuse, uv).rgb;
        vec3 specularCol = texture(material.specular, uv).rgb;

        //环境光
        vec3 ambient = light.ambient * diffuseCol;

        //漫反射
        vec3 normalDir = normalize(normalWS);
        vec3 diffuse = max(0.0, dot(normalDir, lightDir)) * light.diffuse * diffuseCol;

        //高光反射
        vec3 viewDir = normalize(cameraPos - positionWS);
        vec3 halfDir = normalize(viewDir + lightDir);
        vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), material.shininess) * light.specular * specularCol;

        vec3 result = (ambient + diffuse + specular);

        //为了创建一种看起来边缘平滑的聚光
        //  需要模拟聚光有一个内圆锥(Inner Cone)和一个外圆锥(Outer Cone)
        float intensity = clamp((theta - light.cutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);  
        result *= intensity;

        return vec3(result);
    }
    else
    {
        return vec3(0.0);
    }
}

void main()
{
    vec3 dirLightResult = CalcDirLight(dirLight);

    vec3 result = dirLightResult;
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        vec3 pointLightResult = CalcPointLight(pointLights[i]);
        result += pointLightResult;
    }
    
    vec3 spotLightResult = CalcSpotLight(spotLight);
    result += spotLightResult;

    FragColor = vec4(result * objectColor, 1.0);
}