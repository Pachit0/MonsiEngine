#version 330 core

out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Color;

void main() {
	FragColor = vec4(v_Position * 0.5f + 0.5f, 1.0f);
	FragColor = v_Color;
}