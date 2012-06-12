
matrix proj;
matrix view;
matrix world;
float3 eye;

struct Light
{
	float3 diffuse;
	float3 ambient;
	//float3 positionInViewSpace;
	float3 directionInViewSpace;
};
Light light;

#define MAX_BONE_MATRICES	256
cbuffer cbBoneMatrices
{
	matrix cbufBoneMatrices[MAX_BONE_MATRICES];
};

Texture2D tex;
SamplerState samp;

Texture2DArray texToonArray;

struct VS_INPUT 
{
	float3 position		: POSITION0;
	float3 normal		: NORMAL;
	float3 diffuse		: COLOR0;
	float3 specular		: COLOR1;
	float3 ambient		: COLOR2;
	float shininess		: SHININESS;
	float2 uv			: TEXTURE;
	uint2 boneIndices	: BONE_INDICES;
	float weight		: WEIGHT;
	float3 offset		: POSITION1;
	uint toonIndex		: TOON_INDEX;
};

struct PS_INPUT
{
	float4 position	: SV_POSITION;
	float3 color	: COLOR0;
	float3 specular	: COLOR1;
	float2 uv		: TEXTURE;
};

// ----------------------------------------------------------------------------
// 頂点と法線にスキニングとワールドビュー変換を行う。
void SkinningAndTransformWorldView(VS_INPUT input, out float4 out_pos, out float3 out_norm)
{
	float4 in_pos = float4(input.position + input.offset, 1.0);

	out_pos = input.weight * mul(in_pos, cbufBoneMatrices[input.boneIndices[0]]);
	out_pos += (1.0 - input.weight) * mul(in_pos, cbufBoneMatrices[input.boneIndices[1]]);
	out_pos = mul(out_pos, world);
	out_pos = mul(out_pos, view);

	out_norm = input.weight * mul(input.normal, (float3x3)cbufBoneMatrices[input.boneIndices[0]]);
	out_norm += (1.0 - input.weight) * mul(input.normal, (float3x3)cbufBoneMatrices[input.boneIndices[1]]);
	out_norm = mul(out_norm, (float3x3)world);
	out_norm = mul(out_norm, (float3x3)view);
}

// ----------------------------------------------------------------------------
// フォン反射モデルに基づくライティング処理。めんどくさいのでとりあえずディレクショナルライト一つのみ。
float3 CalcDiffuse(float3 norm, float3 diffuse)
{
	// Kd : 演算結果。
	// Dd : マテリアルのディフューズ反射係数。
	// Cd : ライトのディフューズ反射成分。
	// N  : 法線。
	// Ldir : 頂点からライトへ向かうベクトル。
	// Kd = sum[Dd * Cd * (N・Ldir)]	; ライトの数分の合計値。
	float3 Ldir = -light.directionInViewSpace;
	float dot_N_Ldir = max(dot(norm, Ldir), 0.0);
	return diffuse * light.diffuse * dot_N_Ldir;
}

float3 CalcDiffuseAmbient(float3 norm, float3 diffuse, float3 ambient)
{
	return CalcDiffuse(norm, diffuse) + ambient * light.ambient;
}

float3 CalcSpecular(float3 pos, float3 norm, float3 specular, float shininess)
{
	// Ks : 演算結果。
	// Ds : マテリアルのスペキュラ反射係数。
	// Cs : ライトのスペキュラ反射成分。
	// N  : 法線。
	// Ldir : 頂点からライトへ向かうベクトル。
	// R  : 入射光の反射ベクトル。
	// V  : 頂点から視線へ向かうベクトル。
	// a  : 鏡面反射指数。shininess
	// Ks = sum[Ds * Cs * max(R・V, 0)^a * (N・Ldir > 0 ? 1 : 0)]
	float3 Ldir = -light.directionInViewSpace;
	float3 R = reflect(light.directionInViewSpace, norm);
	float3 V = normalize(float3(0, 0, 0) - pos);

	shininess += 0.0001;	
		// max(dot()), shininess 両方 0 だと結果が非数になってしまう為、少しだけ値を足して shininess が 0 にならない様にしておく。
		// ※pow(0, 0)は処理系依存らしい。引数が定数だと、コンパイル時に 1 に置き換えられてしまう様なので、引数が変数の時と結果が異なる。いずれにしても避けなければならない。
		// ※※shininess 自体、見た目で値を決めていくものだからとの事（MJさん談）。
	float pow_dot_R_V_a = pow_dot_R_V_a = pow(max(dot(R, V), 0), shininess);

	float dot_N_Ldir = dot(norm, Ldir);
	if (dot_N_Ldir > 0)
		dot_N_Ldir = 1;
	else
		dot_N_Ldir = 0;

//	return specular * float3(1, 1, 1) * pow_dot_R_V_a * dot_N_Ldir;
	return specular * light.diffuse * pow_dot_R_V_a * dot_N_Ldir;
}

// ----------------------------------------------------------------------------
// ライティング無し。
PS_INPUT VS_None(VS_INPUT input) 
{
	PS_INPUT output;

	float3 norm;
	SkinningAndTransformWorldView(input, output.position, norm);
	output.position = mul(output.position, proj);

	output.color = input.diffuse;
	output.uv = input.uv;

	return output;
}

// ----------------------------------------------------------------------------
// グーローシェーディング。
// ジオメトリシェーダを省略すると、Direct3D10が勝手に色を補間してグーローシェーディングになる為、
// 頂点シェーダでライティング計算を行う（その為には頂点シェーダの入力に法線が必要）。
PS_INPUT VS_Gouraud(VS_INPUT input) 
{
	PS_INPUT output;

	float3 norm;
	SkinningAndTransformWorldView(input, output.position, norm);

	output.color = CalcDiffuseAmbient(norm, input.diffuse, input.ambient);
	output.specular = CalcSpecular(output.position.xyz / output.position.w, input.normal, input.specular, input.shininess);
	output.uv = input.uv;

	output.position = mul(output.position, proj);

	return output;
}

// ----------------------------------------------------------------------------
// フラットシェーディング。
// 法線をジオメトリシェーダで計算し、その法線でライティングする。
// トライアングルの各頂点カラーは同じ法線で計算され、同じ色になる為、フラットシェーディングとなる。
// ただし、ディレクショナルライト以外のライトを扱った場合は、光源から頂点の距離が変化して
// 色が変わってしまう為、何か別の処理を行わなければならない。
struct GS_Flat_INPUT
{
	float4 position	: SV_POSITION;
	float3 diffuse	: COLOR0;
	float3 specular	: COLOR1;
	float3 ambient	: COLOR2;
	float shininess	: SHININESS;
	float2 uv		: TEXTURE;
};

GS_Flat_INPUT VS_Flat(VS_INPUT input) 
{
	GS_Flat_INPUT output;

	float3 norm;
	SkinningAndTransformWorldView(input, output.position, norm);

	output.diffuse = input.diffuse;
	output.specular = input.specular;
	output.ambient = input.ambient;
	output.shininess = input.shininess;
	output.uv = input.uv;

	return output;
}

[maxvertexcount(3)]
void GS_Flat(triangle GS_Flat_INPUT input[3], inout TriangleStream<PS_INPUT> OutputStream) 
{
	PS_INPUT output;

	// 法線の計算。inputは同次座標になっているのでw成分で割ってから計算。
	float3 position0 = (input[0].position.xyz / input[0].position.w);
	float3 position1 = (input[1].position.xyz / input[1].position.w);
	float3 position2 = (input[2].position.xyz / input[2].position.w);
	float3 norm = normalize(cross(position1 - position0, position2 - position0));

	// 色を決定して出力。
	output.position = mul(input[0].position, proj);
	output.color = CalcDiffuseAmbient(norm, input[0].diffuse, input[0].ambient);
	output.specular = CalcSpecular(position0, norm, input[0].specular, input[0].shininess);
	output.uv = input[0].uv;
	OutputStream.Append(output);

	output.position = mul(input[1].position, proj);
	output.color = CalcDiffuseAmbient(norm, input[1].diffuse, input[1].ambient);
	output.specular = CalcSpecular(position0, norm, input[1].specular, input[1].shininess);
	output.uv = input[1].uv;
	OutputStream.Append(output);

	output.position = mul(input[2].position, proj);
	output.color = CalcDiffuseAmbient(norm, input[2].diffuse, input[2].ambient);
	output.specular = CalcSpecular(position0, norm, input[2].specular, input[2].shininess);
	output.uv = input[2].uv;
	OutputStream.Append(output);
	
	OutputStream.RestartStrip();
}

// ----------------------------------------------------------------------------
// ライティングなし／グーロー／フラットシェーディング共用ピクセルシェーダ。
float4 PS_Default(PS_INPUT input) : SV_TARGET
{
	return saturate(tex.Sample(samp, input.uv) * float4(input.color, 1.0) + float4(input.specular, 1.0));
}

// ----------------------------------------------------------------------------
// フォンシェーディング。
// ピクセルシェーダの入力パラメータは、頂点／ジオメトリシェーダの出力パラメータから自動的に補間してくれる模様。
// 法線を出力しておけば、各頂点間のピクセルの法線も補間してくれる。ただし、線形補間で正規化されていないと思われるので normalize する。
struct PS_Phong_INPUT
{
	float4 position	: SV_POSITION;
	float3 positionView : POSITION;
	float3 normal	: NORMAL;
	float3 diffuse	: COLOR0;
	float3 specular	: COLOR1;
	float3 ambient	: COLOR2;
	float shininess	: SHININESS;
	float2 uv		: TEXTURE;
};

PS_Phong_INPUT VS_Phong(VS_INPUT input) 
{
	PS_Phong_INPUT output;

	float3 norm;
	SkinningAndTransformWorldView(input, output.position, norm);

	output.positionView = output.position.xyz / output.position.w;
	output.position = mul(output.position, proj);
	output.normal = norm;
	output.diffuse = input.diffuse;
	output.specular = input.specular;
	output.shininess = input.shininess;
	output.ambient = input.ambient;
	output.uv = input.uv;

	return output;
}

float4 PS_Phong(PS_Phong_INPUT input) : SV_TARGET
{
	float3 norm = normalize(input.normal);
	float3 color = CalcDiffuseAmbient(norm, input.diffuse, input.ambient);
	float3 specular = CalcSpecular(input.positionView, norm, input.specular, input.shininess);
	return saturate(tex.Sample(samp, input.uv) * float4(color, 1.0) + float4(specular, 1.0));
}

// ----------------------------------------------------------------------------
// トゥーンシェーディング。
struct PS_Toon_INPUT
{
	float4 position	: SV_POSITION;
	float3 positionView : POSITION;
	float3 normal	: NORMAL;
	float3 diffuse	: COLOR0;
	float3 specular	: COLOR1;
	float3 ambient	: COLOR2;
	float shininess	: SHININESS;
	float2 uv		: TEXTURE;
	uint toonIndex	: TOON_INDEX;
};

PS_Toon_INPUT VS_Toon(VS_INPUT input) 
{
	PS_Toon_INPUT output;

	float3 norm;
	SkinningAndTransformWorldView(input, output.position, norm);

	output.positionView = output.position.xyz / output.position.w;
	output.position = mul(output.position, proj);
	output.normal = norm;
	output.diffuse = input.diffuse;
	output.specular = input.specular;
	output.shininess = input.shininess;
	output.ambient = input.ambient;
	output.uv = input.uv;
	output.toonIndex = input.toonIndex;

	return output;
}

float4 CalcToon(float3 norm, float3 diffuse, float3 ambient, uint toonIndex)
{
	// 法線と光源ベクトルの内積から、トゥーン用テクスチャの v を計算する。
	float3 Ldir = -light.directionInViewSpace;
	float cosT = dot(norm, Ldir);
	float toon_v = saturate((-cosT + 1.0) * 0.5);
	float4 color = float4(diffuse * light.diffuse + ambient * light.ambient, 1) * texToonArray.Sample(samp, float3(0, toon_v, toonIndex));

	return color;
}

float4 PS_Toon(PS_Toon_INPUT input) : SV_TARGET
{
	float3 norm = normalize(input.normal);
	float4 color = CalcToon(norm, input.diffuse, input.ambient, input.toonIndex);
	float3 specular = CalcSpecular(input.positionView, norm, input.specular, input.shininess);

	float4 result = saturate(tex.Sample(samp, input.uv) * color + float4(specular, 1.0));

	return result;
}

// ----------------------------------------------------------------------------
// ボーン描画用。
struct VS_Bone_INPUT
{
	float3 position		: POSITION;
	float3 color		: COLOR;
};

struct PS_Bone_INPUT
{
	float4 position	: SV_POSITION;
	float3 color	: COLOR;
	float2 uv		: TEXTURE;
};

PS_Bone_INPUT VS_Bone(VS_Bone_INPUT input)
{
	PS_Bone_INPUT output;

	float4 pos4 = float4(input.position, 1.0);

	output.position = mul(pos4, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.color = input.color;

	return output;
}

float4 PS_Bone(PS_Bone_INPUT input) : SV_TARGET
{
	return float4(input.color, 1.0);
}
