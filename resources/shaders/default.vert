#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 worldPosition;
out vec3 worldNormal;


uniform mat4 modelMatrix;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mat3 normalMatrix;

void main()
{
    worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));

    worldNormal = normalize(normalMatrix * vertexNormal);

    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
