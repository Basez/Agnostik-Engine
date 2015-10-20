//cbuffer PerApplication : register(b0)
//{
//	matrix projectionMatrix;
//}

//cbuffer PerFrame : register(b1)
//{
//	matrix viewMatrix;
//}

// TODO: fix registers when restoring Cbuffers
cbuffer PerObject : register(b0)
{
	matrix modelViewProjectionMatrix;
	//matrix modelMatrix;
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

	OUT.position = mul(modelViewProjectionMatrix, float4(IN.position, 1.0f));
	OUT.uv = IN.uv;
	return OUT;
}