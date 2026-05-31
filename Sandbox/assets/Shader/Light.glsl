#shadertype vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 lightModel;
uniform mat4 lightView;
uniform mat4 lightProjection;

void main() {
    gl_Position = lightProjection * lightView * lightModel * vec4(aPos, 1.0f);
}

#shadertype fragment
#version 330 core

uniform vec4 lightColor;

out vec4 FragColor;

void main() {
	FragColor = lightColor;
}