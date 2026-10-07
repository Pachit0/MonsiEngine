#shadertype vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_ViewProjection;

out vec3 texCoords;

void main() {
    vec4 pos = u_ViewProjection * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}

#shadertype fragment
#version 330 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube u_Skybox;

void main() {
	FragColor = texture(u_Skybox, texCoords);
}