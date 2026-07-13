#shadertype vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceTransform;
layout(location = 7) in vec4 a_InstanceColor;

uniform mat4 u_ViewProjection;

out vec2 TexCoords;
out vec4 InstanceColor;
out vec3 v_FragPos;
out vec3 v_Normal;

void main()
{
    TexCoords = a_TexCoord;
    InstanceColor = a_InstanceColor;
    
    vec4 worldPos = a_InstanceTransform * vec4(a_Position, 1.0);
    v_FragPos = vec3(worldPos);

    v_Normal = mat3(a_InstanceTransform) * a_Normal;

    gl_Position = u_ViewProjection * worldPos;
}

#shadertype fragment
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;

    sampler2D specularMap;
    sampler2D normalMap;

    float hasDiffuseMap;
    float hasSpecularMap;
    float hasNormalMap;
}; 

struct DirectionalLight {
    vec3 Direction;
    vec3 Color;
    float Intensity;
};

struct PointLight {
    vec3 Position;
    vec3 Color;
    float Intensity;
    float Radius;
};

uniform DirectionalLight u_MainLight;
uniform int u_PointLightCount;
uniform Material material;
uniform PointLight u_PointLights[32];
uniform vec3 u_ViewPos;
uniform sampler2D texture_diffuse1;

in vec2 TexCoords;
in vec4 InstanceColor;
in vec3 v_FragPos;
in vec3 v_Normal;

out vec4 FragColor;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    if (dot(light.Direction, light.Direction) < 0.0001) return vec3(0.0);

    vec3 lightDir = normalize(-light.Direction);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff * material.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.Color * light.Intensity * spec * material.specular;

    return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 ToLight = light.Position - fragPos;
    float distance = length(ToLight);

    if (distance < 0.0001) return vec3(0.0);

    vec3 lightDir = ToLight / distance;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff * material.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.Color * light.Intensity * spec * material.specular;

    float attenuation = clamp(1.0 - (distance / light.Radius), 0.0, 1.0);

    return (diffuse + specular) * attenuation;
}

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords) * InstanceColor;
    
    if (texColor.a < 0.5)
    {
        discard;
    }

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    
    vec3 lightResult = material.ambient;
    
    lightResult += CalculateDirectionalLight(u_MainLight, norm, viewDir);
    
    for(int i = 0; i < u_PointLightCount; i++) {
        lightResult += CalculatePointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    }

    FragColor = vec4(texColor.rgb * lightResult, texColor.a);
}