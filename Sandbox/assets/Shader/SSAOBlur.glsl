#shadertype vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}

#shadertype fragment
#version 450 core

layout(location = 0) out float o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_SSAOInput;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_SSAOInput, 0));
    float result = 0.0;

    for (int x = -2; x < 2; ++x)
    {
        for (int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_SSAOInput, v_TexCoord + offset).r;
        }
    }

    o_Color = result / 16.0;
}
