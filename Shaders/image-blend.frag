#version 450 core

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;

layout(binding=0) uniform sampler2D oldTexture;
layout(binding=1) uniform sampler2D newTexture;

uniform float mixValue;

void main()
{
    fragColor=mix(texture(oldTexture,texcoord),texture(newTexture,texcoord),mixValue);
}