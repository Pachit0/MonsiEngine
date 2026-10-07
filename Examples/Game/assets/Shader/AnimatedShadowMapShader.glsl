#shadertype vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 3) in ivec4 a_BoneIDs;
layout(location = 4) in vec4 a_Weights;

const int MAX_BONES = 100;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Transform;
uniform mat4 u_FinalBoneMatrices[MAX_BONES];

void main()
{
    mat4 boneTransform = mat4(0.0);
    bool isSkinned = false;

    for (int i = 0; i < 4; ++i)
    {
        if (a_BoneIDs[i] >= 0 && a_BoneIDs[i] < MAX_BONES)
        {
            boneTransform += u_FinalBoneMatrices[a_BoneIDs[i]] * a_Weights[i];
            isSkinned = true;
        }
    }

    if (!isSkinned)
        boneTransform = mat4(1.0);

    vec4 skinnedPosition = boneTransform * vec4(a_Position, 1.0);
    gl_Position = u_LightSpaceMatrix * u_Transform * skinnedPosition;
}

#shadertype fragment
#version 450 core

void main()
{
    // Depth-only pass
}