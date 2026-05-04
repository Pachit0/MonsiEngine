#shadertype vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}

#shadertype fragment
#version 330 core

out vec4 FragColor;
uniform vec4 u_Color;

void main() {
	FragColor = u_Color;
}