#version 450 core

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;

layout(pixel_center_integer) in vec4 gl_FragCoord;

void main()
{
    fragColor=vec4(texcoord.x,texcoord.y,0.25,1.0);
}