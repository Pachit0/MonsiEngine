#shadertype vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceTransform;
layout(location = 7) in vec4 a_InstanceColor;

uniform mat4 u_ViewProjection;
uniform mat4 u_LightSpaceMatrix;

out vec2 TexCoords;
out vec4 InstanceColor;
out vec3 v_FragPos;
out vec3 v_Normal;
out vec4 v_FragPosLightSpace;

void main()
{
    TexCoords = a_TexCoord;
    InstanceColor = a_InstanceColor;
    
    vec4 worldPos = a_InstanceTransform * vec4(a_Position, 1.0);
    v_FragPos = vec3(worldPos);

    v_Normal = mat3(a_InstanceTransform) * a_Normal;
    v_FragPosLightSpace = u_LightSpaceMatrix * worldPos;

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

uniform sampler2DShadow u_ShadowMap;
uniform float u_ShadowIntensity;
uniform float u_ShadowFilterRadius = 1.5;

in vec2 TexCoords;
in vec4 InstanceColor;
in vec3 v_FragPos;
in vec3 v_Normal;
in vec4 v_FragPosLightSpace;

out vec4 FragColor;

const vec2 POISSON_DISK[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
);

float RandomGradient(vec2 seed)
{
    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);
}

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

float CalculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float bias = max(0.002 * (1.0 - dot(normal, lightDir)), 0.0007);
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);

    float randomAngle = RandomGradient(gl_FragCoord.xy) * 6.28318530718;
    float cosTheta = cos(randomAngle);
    float sinTheta = sin(randomAngle);
    mat2 rotMatrix = mat2(cosTheta, sinTheta, -sinTheta, cosTheta);

    float lit = 0.0;

    for (int i = 0; i < 16; ++i)
    {
        vec2 sampleOffset = rotMatrix * POISSON_DISK[i] * u_ShadowFilterRadius * texelSize;
        lit += texture(u_ShadowMap, vec3(projCoords.xy + sampleOffset, projCoords.z - bias));
    }

    lit /= 16.0;

    return 1.0 - lit;
}

void main()
{
    vec4 texColor;
    if (material.hasDiffuseMap > 0.5)
        texColor = texture(texture_diffuse1, TexCoords) * InstanceColor;
    else
        texColor = vec4(material.diffuse, 1.0) * InstanceColor;

    if (texColor.a < 0.5)
        discard;

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 lightResult = material.ambient;

    float shadow = CalculateShadow(v_FragPosLightSpace, norm, normalize(-u_MainLight.Direction));
    
    shadow *= clamp(u_ShadowIntensity, 0.0, 1.0);

    lightResult += (1.0 - shadow) * CalculateDirectionalLight(u_MainLight, norm, viewDir);

    for(int i = 0; i < u_PointLightCount; i++) {
        lightResult += CalculatePointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    }

    FragColor = vec4(texColor.rgb * lightResult, 1.0);
}