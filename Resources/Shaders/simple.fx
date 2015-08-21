cbuffer MatrixProjection : register(b0)
{
    matrix projectionMatrix;
}
 
cbuffer MatrixView : register(b1)
{
    matrix viewMatrix;
}
 
cbuffer MatrixWorld : register(b2)
{
    matrix worldMatrix;
}

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
	
    matrix mvp = mul( projectionMatrix, mul( viewMatrix, worldMatrix ) );
    output.position = mul( mvp, input.position );
	
    output.tex = input.tex;
    
    return output;
}

float4 PShader(PixelInputType input) : SV_Target
{
    return shaderTexture.Sample(SampleType, input.tex);
}
