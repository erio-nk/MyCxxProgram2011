
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
//	// サンプラなし。
//    uint width, levels;
//    Tex.GetDimensions(width, levels);           // 幅
//    float3 texCoord = float3(input.uv * width, 0); // ミップマップ・レベル「0」
//	return saturate(tex.Load(texCoord));

	// サンプラあり。sampが未設定の場合は、デフォルトの処理でやってくれるっぽい。
	return saturate(tex.Sample(samp, input.uv));
}
