/**
	適当シェーダ
 */
 
// トランスフォーム行列
float4x4 mWorld;
float4x4 mView;
float4x4 mProj;

// 頂点シェーダの入力パラメータ
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

// ピクセルシェーダの入力パラメータ
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
	
	// 頂点カラーを赤にする
	//psIn.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	psIn.Color = vsIn.Color;
	
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

