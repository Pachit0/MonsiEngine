#shadertype vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec2 v_TexCoord;

void main() {
    v_TexCoord = aTexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
}

#shadertype fragment
#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_Scale;

void main() {
	FragColor = texture(u_Texture, v_TexCoord * u_Scale) * u_Color;
}