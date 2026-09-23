#shadertype vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform mat4 u_NormalMatrix;

out vec3 v_ViewPosition;
out vec3 v_ViewNormal;

void main()
{
    vec4 viewPos = u_View * u_Model * vec4(a_Position, 1.0);
    v_ViewPosition = viewPos.xyz;
    v_ViewNormal = normalize(mat3(u_NormalMatrix) * a_Normal);

    gl_Position = u_Projection * viewPos;
}

#shadertype fragment
#version 450 core

layout(location = 0) out vec4 o_Position;
layout(location = 1) out vec4 o_Normal;

in vec3 v_ViewPosition;
in vec3 v_ViewNormal;

void main()
{
    o_Position = vec4(v_ViewPosition, 1.0);
    o_Normal = vec4(normalize(v_ViewNormal), 0.0);
}
