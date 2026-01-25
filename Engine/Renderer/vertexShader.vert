#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec3 v_Position;
out vec4 v_Color;

void main() {
    v_Position = aPos;
    v_Color = aColor;
    gl_Position = vec4(aPos, 1.0);
}