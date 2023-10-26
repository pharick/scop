#version 330

layout (location = 0) in vec4 position;

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;

flat out vec4 vertColor;

void main()
{
    vec4 cameraPos = modelToCameraMatrix * position;
    gl_Position = cameraToClipMatrix * cameraPos;
    vertColor = vec4(0.5f * (position.xyz + 1.0f), 1.0f);
}
