#version 410 core

layout (isolines, equal_spacing, ccw) in;

const float kPi = 3.14159265358979323846f;

uniform mat3 model;

uniform float windowWidth;
uniform float windowHeight;

void main()
{
    vec4 params = gl_in[0].gl_Position;

    vec3 c = model * vec3(2.0f * params.x / windowWidth - 1.0f,
                          2.0f * params.y / windowHeight - 1.0f,
                          1.0f);
    c.z = 0.0f;

    // A circle is scaled into an oval when the viewport is not a perfect square.
    float a = 2.0f * params.z / windowWidth;
    float b = 2.0f * params.z / windowHeight;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // Use u here because the isolines mode only connects mesh grids horizontally!
    // If we use v here, theta only differs vertically and cooresponding vertices will not be connected!
    float theta = 2 * kPi * u;

    // Try the following and see the difference:
    // 1. Use v in theta and there will be NO output;
    // 2. Add "point_mode" to line 3 as "layout (isolines, equal_spacing, ccw, point_mode) in";
    // 3. Run the same program again and see the dot output!

    gl_Position = vec4(vec3(a * cos(theta), b * sin(theta), 0.0f) + c, 1.0f);
}
