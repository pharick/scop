#version 330

smooth in vec2 vertTextureCoords;

out vec4 outputColor;

uniform sampler2D textureSampler;

void main()
{
    outputColor = texture2D(textureSampler, vertTextureCoords);
}
