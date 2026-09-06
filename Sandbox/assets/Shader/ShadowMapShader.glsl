#shadertype vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_LightSpaceMatrix * u_Transform * vec4(a_Position, 1.0);
}

#shadertype fragment
#version 450 core

void main()
{
    // Depth-only pass - no color attachment is bound (glDrawBuffer(GL_NONE)
    // in OpenGLShadowMap::Init), so gl_FragDepth from gl_Position.z is all
    // that gets written.
}
