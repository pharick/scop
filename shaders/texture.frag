#version 330

in vec2 vertTextureCoords;

out vec4 outputColor;

uniform sampler2D sampler;

void main()
{
    outputColor = texture2D(sampler, vertTextureCoords);
}
