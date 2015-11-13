//cbuffer PerApplication : register(b0)
//{
//	float4x4 projectionMatrix;
//}

//cbuffer PerFrame : register(b1)
//{
//	float4x4 viewMatrix;
//}

cbuffer PerObject : register(b0)
{
	float4x4 modelViewProjectionMatrix;
	float4x4 modelMatrix;
}

struct InputData
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
	float3 biTangent : BITANGENT;
};

struct OutputData
{
	float2 uv : TEXCOORD0;
	float4 normalWS : TEXCOORD1;
	float4 positionWS : TEXCOORD2;
	float4 tangentWS : TEXCOORD3;
	float4 biTangentWS : TEXCOORD4;
	float4 position : SV_POSITION;	// must be last as its consumed
};

OutputData Main(InputData IN)
{
	OutputData OUT;

	OUT.position = mul(modelViewProjectionMatrix, float4(IN.position, 1.0f));
	OUT.normalWS = mul(modelMatrix, float4(IN.normal, 0.0f)); 
	OUT.positionWS = mul(modelMatrix, float4(IN.position, 1.0f));
	OUT.tangentWS = mul(modelMatrix, float4(IN.tangent, 0.0f)); 
	OUT.biTangentWS = mul(modelMatrix, float4(IN.biTangent, 0.0f)); 
	OUT.uv = IN.uv;

	return OUT;
}