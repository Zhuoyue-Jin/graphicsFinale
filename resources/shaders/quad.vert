#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position (NDC: -1 to 1)
out vec2 fragCoord; // Pass screen space coordinates to the fragment shader

void main() {
    fragCoord = aPos.xy;
    gl_Position = vec4(aPos, 1.0);
}
