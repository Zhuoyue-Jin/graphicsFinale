#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 rayDirection;

void main()
{
    // Pass the camera-space ray direction to the fragment shader
    vec4 viewPos = inverse(projection * view) * vec4(aPos, 1.0);
    rayDirection = normalize(viewPos.xyz / viewPos.w);

    // Set vertex position to clip space
    gl_Position = vec4(aPos, 1.0);
}
