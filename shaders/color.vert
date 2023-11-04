#version 330

layout (location = 0) in vec4 position;

uniform mat4 modelToWorldMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 cameraToClipMatrix;

flat out vec4 vertColor;

void main()
{
    vec4 worldPos = modelToWorldMatrix * position;
    vec4 cameraPos = worldToCameraMatrix * worldPos;
    gl_Position = cameraToClipMatrix * cameraPos;
    vertColor = vec4(0.5f * (worldPos.xyz + 1.0f), 1.0f);
}
