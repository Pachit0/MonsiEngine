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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform mat4 u_Projection;
uniform vec3 u_Samples[64];
uniform int u_KernelSize;
uniform float u_Radius;
uniform float u_Bias;
uniform vec2 u_NoiseScale;

uniform int u_DebugMode;
uniform float u_FarPlane = 100.0;

void main()
{
    vec3 fragPos = texture(gPosition, v_TexCoord).xyz;

    if (u_DebugMode == 1)
    {
        float linearDepth = abs(fragPos.z) / u_FarPlane;
        o_Color = clamp(linearDepth, 0.0, 1.0);
        return;
    }

    vec3 normal = normalize(texture(gNormal, v_TexCoord).xyz);
    vec3 randomVec = normalize(texture(texNoise, v_TexCoord * u_NoiseScale).xyz);

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < u_KernelSize; ++i)
    {
        vec3 samplePos = TBN * u_Samples[i];
        samplePos = fragPos + samplePos * u_Radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = u_Projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPosition, offset.xy).z;

        float rangeCheck = smoothstep(0.0, 1.0, u_Radius / max(abs(fragPos.z - sampleDepth), 0.0001));
        occlusion += (sampleDepth >= samplePos.z + u_Bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / float(u_KernelSize));

    o_Color = occlusion;
}