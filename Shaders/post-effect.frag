#version 450 core

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;

layout(binding=0) uniform sampler2D finalTexture;

#define SHARPEN 1
#define BLUR 2
#define EDGEDETECT 3

uniform int postEffectType;


const float offset=1.0/1000;
const vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),
        vec2( 0.0f,    offset),
        vec2( offset,  offset),
        vec2(-offset,  0.0f),  
        vec2( 0.0f,    0.0f),  
        vec2( offset,  0.0f),  
        vec2(-offset, -offset),
        vec2( 0.0f,   -offset),
        vec2( offset, -offset) 
    );

float sharpenKernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
);

float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

void main()
{
    if(postEffectType==SHARPEN)
    { 
        vec3 col=vec3(0.0);
	    vec3 sampleTex[9];
	    for(int i=0;i<9;i++)
		    sampleTex[i]=vec3(texture(finalTexture,texcoord.st+offsets[i]));

		for(int i=0;i<9;i++)
		col+=sampleTex[i]*sharpenKernel[i];

		fragColor=vec4(col,1.0);
    }
    else if(postEffectType==BLUR)
    {
        vec3 col=vec3(0.0);
	    vec3 sampleTex[9];
	    for(int i=0;i<9;i++)
		    sampleTex[i]=vec3(texture(finalTexture,texcoord.st+offsets[i]));

		for(int i=0;i<9;i++)
		col+=sampleTex[i]*blurKernel[i];

		fragColor=vec4(col,1.0);
    }
    else if(postEffectType==EDGEDETECT)
    {
	    float Gx[9]=float[](
	    -1,0,1,
	    -2,0,2,
	    -1,0,1
	    );

	    float Gy[9]=float[](
	    -1,-2,-1,
	     0, 0,0,
	    1, 2,1
	    );
        vec3 sampleTex[9];
	    float edgeX;
	    float edgeY;
	    for(int i=0;i<9;i++)
		    sampleTex[i]=vec3(texture(finalTexture,texcoord.st+offsets[i]));

	    for(int i=0;i<9;i++)
	    {
		    float luminance = 0.2126 * sampleTex[i].r + 0.7152 * sampleTex[i].g + 0.0722 * sampleTex[i].b;
		    edgeX+=luminance*Gx[i];
		    edgeY+=luminance*Gy[i];
	    }
	    float edge=1-abs(edgeX)-abs(edgeY);

	    fragColor=(1-edge)*vec4(0.0,0.0,0.0,1.0)+edge*vec4(1.0);
    }
    else
        fragColor=texture(finalTexture,texcoord);
}