#include "variables.fx"

Texture2D shaderTexture;
SamplerState SampleType;


struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType VShader(VertexInputType input)
{
    PixelInputType output;
	
	input.position.w = 1.0f;
    output.position = mul(_MatrixMVP, input.position );
    output.tex = input.tex;
    
    return output;
}

float4 PShader(PixelInputType input) : SV_Target
{
    return shaderTexture.Sample(SampleType, input.tex) * _Color;
}
