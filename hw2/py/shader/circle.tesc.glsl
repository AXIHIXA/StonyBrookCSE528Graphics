#version 410 core

layout (vertices = 1) out;

void main()
{
    gl_TessLevelOuter[0] = 64.0f;
    gl_TessLevelOuter[1] = 64.0f;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
