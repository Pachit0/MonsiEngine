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

void main()
{
    TexCoords = a_TexCoord;
    InstanceColor = a_InstanceColor;

    gl_Position = u_ViewProjection * a_InstanceTransform * vec4(a_Position, 1.0);
}

#shadertype fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec4 InstanceColor;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords) * InstanceColor;
}