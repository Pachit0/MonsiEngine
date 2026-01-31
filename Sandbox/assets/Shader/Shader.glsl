#shadertype vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Position;
out vec4 v_Color;

void main() {
    v_Position = aPos;
    v_Color = aColor;
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}

#shadertype fragment
#version 330 core

out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Color;

void main() {
	FragColor = vec4(v_Position * 0.5f + 0.5f, 1.0f);
	FragColor = v_Color;
}