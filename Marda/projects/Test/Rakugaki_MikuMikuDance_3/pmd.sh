
matrix proj;
matrix view;
matrix world;

#define MAX_BONE_MATRICES	256
cbuffer cbBoneMatrices
{
	matrix cbufBoneMatrices[MAX_BONE_MATRICES];
};

Texture2D tex;
SamplerState samp;

struct VS_INPUT 
{
	float3 position		: POSITION;
	float3 color		: COLOR;
	float2 uv			: TEXTURE;
	uint2 boneIndices	: BONE_INDICES;
	float weight		: WEIGHT;
};

struct VS_bone_INPUT
{
	float3 position		: POSITION;
	float3 color		: COLOR;
};

struct PS_INPUT
{
	float4 position	: SV_POSITION;
	float3 color	: COLOR;
	float2 uv		: TEXTURE;
};

PS_INPUT VS(VS_INPUT input) 
{
	PS_INPUT output;

	float4 pos4 = float4(input.position, 1.0);

	output.position = input.weight * mul(pos4, cbufBoneMatrices[input.boneIndices[0]]);
	output.position+= (1.0 - input.weight) * mul(pos4, cbufBoneMatrices[input.boneIndices[1]]);
	output.position = mul(output.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.color = input.color;
	output.uv = input.uv;

	return output;
}

PS_INPUT VS_bone(VS_bone_INPUT input)
{
	PS_INPUT output;

	float4 pos4 = float4(input.position, 1.0);

	output.position = mul(pos4, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.color = input.color;

	return output;
}

float4 PS_default(PS_INPUT input) : SV_TARGET
{
	return float4(input.color, 1.0);
}

float4 PS_texture(PS_INPUT input) : SV_TARGET
{
	return saturate(tex.Sample(samp, input.uv));
}
