#shadertype vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_FragPos;
out vec3 v_Normal;

void main() {
    v_FragPos = vec3(u_Model * vec4(aPos, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}

#shadertype fragment
#version 330 core

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
uniform PointLight u_PointLights[32];

out vec4 FragColor;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    return light.Color * light.Intensity * diff;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    
    float distance = length(light.Position - fragPos);
    float attenuation = clamp(1.0 - (distance / light.Radius), 0.0, 1.0);
    
    return light.Color * light.Intensity * diff * attenuation;
}

void main() {
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = vec3(0.0);
    
    vec3 finalColor = vec3(0.0);
    
    finalColor += vec3(0.1); 

    finalColor += CalculateDirectionalLight(u_MainLight, norm, viewDir);

    for(int i = 0; i < u_PointLightCount; i++) {
        finalColor += CalculatePointLight(u_PointLights[i], norm, v_FragPos);
    }

    FragColor = vec4(finalColor, 1.0);
}