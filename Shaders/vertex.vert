#version 450 core

layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexcoord;
layout(location=3) in vec3 inTangent;
layout(location=4) in vec3 inBinormal;

layout(location=0) out vec2 texcoord;

void main()
{
    gl_Position=vec4(inPosition,1.0);
    texcoord=vec2(inPosition.x,inPosition.y)*0.5+0.5;
}