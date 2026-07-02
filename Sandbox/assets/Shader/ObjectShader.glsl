#shadertype vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in mat4 a_Transform; 
layout(location = 6) in vec4 a_Color;
layout(location = 7) in float a_TexIndex;
layout(location = 8) in vec3 a_Normal; 

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out float v_TexIndex;
out vec3 v_FragPos;
out vec3 v_Normal;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    
    vec4 worldPos = a_Transform * vec4(a_Position, 1.0);
    v_FragPos = vec3(worldPos);
    v_Normal = mat3(transpose(inverse(a_Transform))) * a_Normal;
    
    gl_Position = u_ViewProjection * worldPos;
}

#shadertype fragment
#version 330 core

layout(location=0) out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
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

uniform Material material;
uniform DirectionalLight u_MainLight;
uniform int u_PointLightCount;
uniform PointLight u_PointLights[32];
uniform vec3 u_ViewPos;
uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_TexCoord;
flat in float v_TexIndex;
in vec3 v_FragPos;
in vec3 v_Normal;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    if (length(light.Direction) == 0.0) return vec3(0.0);
    
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
    if (length(ToLight) < 0.0001) return vec3(0.0);
    
    vec3 lightDir = normalize(ToLight);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.Color * light.Intensity * diff * material.diffuse;
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.Color * light.Intensity * spec * material.specular;
    
    float distance = length(ToLight);
    float attenuation = clamp(1.0 - (distance / light.Radius), 0.0, 1.0);
    
    return (diffuse + specular) * attenuation;
}

void main()
{
    int l_TexIndex = int(v_TexIndex);
    vec4 l_Texture = texture(u_Textures[l_TexIndex], v_TexCoord);

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    
    vec3 lightResult = material.ambient;
    
    lightResult += CalculateDirectionalLight(u_MainLight, norm, viewDir);
    
    for(int i = 0; i < u_PointLightCount; i++) {
        lightResult += CalculatePointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    }

    vec4 texColor = v_Color * l_Texture;
    color = vec4(texColor.rgb * lightResult, texColor.a);
}