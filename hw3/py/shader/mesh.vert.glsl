#version 410 core

// The "a" prefix stands for "attribute".
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

// These out variables will be passed along the pipeline
// and be refered with a uniform name in all shader stages,
// thus we add an "our" prefix.
out vec3 ourFragPos;
out vec3 ourNormal;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int displayMode;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
    ourFragPos = vec3(model * vec4(aPosition, 1.0f));
    ourNormal = vec3(transpose(inverse(model)) * vec4(aNormal, 1.0f));
    ourColor = aColor;
}
