#version 330

layout (location = 0) in vec4 position;

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;

smooth out vec2 vertTextureCoords;

void main()
{
    vec4 cameraPos = modelToCameraMatrix * position;
    gl_Position = cameraToClipMatrix * cameraPos;
    vertTextureCoords = vec2(position.y, position.z);
}
