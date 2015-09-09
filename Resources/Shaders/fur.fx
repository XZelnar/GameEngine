#include "variables.fx"
 
cbuffer FurInfo
{
    float length;
	float3 gravity;
}
 
cbuffer IterationInfo
{
    float layerOffset;
	float3 ___blank;
}

Texture2D shaderTexture;
Texture2D colorMask;
SamplerState SampleType;


struct VertexInputType
{
    float4 position : POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float alphaThreshold : COLOR;
	float3 normal : NORMAL;
	float light : TEXCOORD1;
};

PixelInputType VShader(VertexInputType input)
{
    PixelInputType output;
	
	input.position += float4(input.normal * layerOffset * length, 0);
	input.position.w = 1.0f;
	
	float3 vGravity = mul(float4(gravity, 0), _Object2World).xyz;
	input.position += float4(vGravity * pow(layerOffset, 3), 0);
	
    output.position = mul(_MatrixMVP, input.position );
	
    output.tex = input.tex;
	output.alphaThreshold = layerOffset * .9;
	
	float3 normal = normalize(mul(float4(input.normal, 0), _Object2World)).xyz;
	output.normal = normal;
	
	output.light = dot(normalize(float4(input.position.xyz - float3(.8, .8, 1), 0)), float4(input.normal, 0));
    
    return output;
}

float4 PShader(PixelInputType input) : SV_Target
{
    float4 fur = shaderTexture.Sample(SampleType, input.tex);
	if (fur.a < input.alphaThreshold)
		return float4(0, 0, 0, 0);
	float4 col = colorMask.Sample(SampleType, input.tex);
	fur.xyz = col.xyz;
		
	float4 res = fur;
	float4 ambient = {0.3, 0.3, 0.3, 0.0};
	ambient = ambient * res;
	float4 diffuse = res;
	res = ambient + diffuse * input.light;
	res.a = fur.a;
		
	if (input.alphaThreshold > .5)
		return res;
	float t = input.alphaThreshold * 2;
	return res * float4(t, t, t, 1);
}
