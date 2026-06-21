#shadertype vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;
layout(location = 4) in float aScale;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_Scale;

void main() {
    v_TexCoord = aTexCoord;
    v_Color = aColor;
    v_TexIndex = aTexIndex;
    v_Scale = aScale;
    gl_Position = u_ViewProjection * vec4(aPos, 1.0);
}

#shadertype fragment
#version 450 core

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_Scale;

uniform sampler2D u_Textures[32];

void main() {

    int l_TexIndex = int(v_TexIndex);
    vec4 l_Texture = texture(u_Textures[int(v_TexIndex)], v_TexCoord);

    color = l_Texture * v_Color;
}