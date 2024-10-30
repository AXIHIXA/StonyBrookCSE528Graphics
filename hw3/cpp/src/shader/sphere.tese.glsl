#version 410 core

layout (quads, equal_spacing, ccw) in;
//layout (quads, equal_spacing, ccw, point_mode) in;

out vec3 ourNormal;
out vec3 ourFragPos;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 center;
uniform float radius;
uniform vec3 color;

const float kPi = 3.14159265358979323846f;

void main()
{
    // need to use gl_in[0] in some way, otherwise tese shader won't be called!
    vec4 WC = gl_in[0].gl_Position;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float phi = 2.0f * kPi * u;
    float theta = kPi * v;

    vec3 pos = center + vec3(radius * sin(theta) * cos(phi), radius * sin(theta) * sin(phi), radius * cos(theta));
    gl_Position = projection * view * model * vec4(pos, 1.0f);

    ourFragPos = vec3(model * vec4(pos, 1.0f));
    ourNormal = vec3(transpose(inverse(model)) * vec4(pos, 1.0f));
    ourColor = color;
}