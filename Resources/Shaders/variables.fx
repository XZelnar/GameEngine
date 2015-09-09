cbuffer _PerFrame
{
	// Time since current scene load
	uniform float4 _Time; // (t/20, t, t*2, t*3)
	uniform float4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
	uniform float4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
	uniform float4 _GameTime; // (t/20, t, t*2, t*3)
	uniform float4 _SinGameTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
	uniform float4 _CosGameTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
	uniform float4 _DeltaTime; // dt, 1/dt, gamedt, 1/gamedt
}

cbuffer _PerCamera
{
    matrix _MatrixV;
    matrix _MatrixP;
    matrix _MatrixVP;
	float3 _WorldSpaceCameraPos;
	float __unused;
}

cbuffer _PerObject
{
	matrix _MatrixMVP;
	matrix _Object2World;
	matrix _World2Object;
}

cbuffer _PerObjectColor
{
	float4 _Color;
}