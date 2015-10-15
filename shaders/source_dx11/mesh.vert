cbuffer PerApplication : register(b0)
{
	matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
	matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
	matrix worldMatrix;
}

struct InputData
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct OutputData
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

OutputData Main(InputData IN)
{
	OutputData OUT;

	matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
	OUT.position = mul(mvp, float4(IN.position, 1.0f));
	OUT.uv = IN.uv;
	return OUT;
}