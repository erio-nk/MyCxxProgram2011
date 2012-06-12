/**
	適当シェーダ
 */
 
// トランスフォーム行列
float4x4 mWorld;
float4x4 mView;
float4x4 mProj;

// ライティング情報
float3 vLightDir;
float4 vLightColor;

// ディフューズ
float4 vDiffuse;

// 頂点シェーダ入力パラメータ
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
};

// ピクセルシェーダ入力パラメータ
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

// 頂点シェーダ
PS_INPUT VS(VS_INPUT vsIn)
{
	PS_INPUT psIn;
	
	// 頂点をトランスフォームする
	psIn.Pos = mul(vsIn.Pos, mWorld);
	psIn.Pos = mul(psIn.Pos, mView);
	psIn.Pos = mul(psIn.Pos, mProj);
	
	// ライティング（ディフューズのみ）
	//psIn.Color = vsIn.Color * vLightColor * saturate(dot(vsIn.Normal, vLightDir));
	psIn.Color = vDiffuse * vLightColor * saturate(dot(vsIn.Normal, vLightDir));
	
	return psIn;
}

// ピクセルシェーダ
float4 PS(PS_INPUT psIn) : SV_Target
{
	return psIn.Color;
}

// テクニック
technique10 Render
{
	// パス（って何？）
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

