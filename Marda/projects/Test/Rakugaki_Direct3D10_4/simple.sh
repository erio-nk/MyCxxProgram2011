
matrix proj;
matrix view;
matrix world;

Texture2D tex;
SamplerState samp;

struct VS_INPUT 
{
	float3 position	: POSITION;
	float2 uv		: TEXTURE;
};

struct PS_INPUT
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXTURE;
};

PS_INPUT VS(VS_INPUT input) 
{
	PS_INPUT output;

	float4 pos4 = float4(input.position, 1.0);
	output.position = mul(pos4, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.uv = input.uv;

    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
//	// �T���v���Ȃ��B
//    uint width, levels;
//    Tex.GetDimensions(width, levels);           // ��
//    float3 texCoord = float3(input.uv * width, 0); // �~�b�v�}�b�v�E���x���u0�v
//	return saturate(tex.Load(texCoord));

	// �T���v������Bsamp�����ݒ�̏ꍇ�́A�f�t�H���g�̏����ł���Ă������ۂ��B
	return saturate(tex.Sample(samp, input.uv));
}
