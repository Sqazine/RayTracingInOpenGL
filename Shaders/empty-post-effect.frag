#version 450 core

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;

layout(binding=0) uniform sampler2D finalTexture;

void main()
{
    fragColor=texture(finalTexture,texcoord);
}