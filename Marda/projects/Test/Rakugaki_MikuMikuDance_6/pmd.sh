
matrix proj;
matrix view;
matrix world;
float3 eye;

struct Light
{
	float3 diffuse;
	float3 ambient;
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
	float4 diffuse		: COLOR0;
	float3 specular		: COLOR1;
	float3 ambient		: COLOR2;
	float shininess		: SHININESS;
	float2 uv			: TEXTURE;
	uint2 boneIndices	: BONE_INDICES;
	float weight		: WEIGHT;
	float3 offset		: POSITION1;
	uint toonIndex		: TOON_INDEX;
	uint edgeFlag		: EDGE_FLAG;
};

struct PS_INPUT
{
	float4 position	: SV_POSITION;
	float3 positionView : POSITION;
	float depth		: DEPTH;
	float3 normal	: NORMAL;
	float4 diffuse	: COLOR0;
	float3 specular	: COLOR1;
	float3 ambient	: COLOR2;
	float shininess	: SHININESS;
	float2 uv		: TEXTURE;
	uint toonIndex	: TOON_INDEX;
	uint edgeFlag	: EDGE_FLAG;
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
//float3 CalcDiffuse(float3 norm, float3 diffuse)
//{
//	// Kd : 演算結果。
//	// Dd : マテリアルのディフューズ反射係数。
//	// Cd : ライトのディフューズ反射成分。
//	// N  : 法線。
//	// Ldir : 頂点からライトへ向かうベクトル。
//	// Kd = sum[Dd * Cd * (N・Ldir)]	; ライトの数分の合計値。
//	float3 Ldir = -light.directionInViewSpace;
//	float dot_N_Ldir = max(dot(norm, Ldir), 0.0);
//	return diffuse * light.diffuse * dot_N_Ldir;
//}
//
//float3 CalcDiffuseAmbient(float3 norm, float3 diffuse, float3 ambient)
//{
//	return CalcDiffuse(norm, diffuse) + ambient * light.ambient;
//}

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
// 共通頂点シェーダ。
PS_INPUT VS(VS_INPUT input) 
{
	PS_INPUT output;

	float3 norm;
	SkinningAndTransformWorldView(input, output.position, norm);

	output.positionView = output.position.xyz / output.position.w;
	output.position = mul(output.position, proj);
	output.depth = output.position.z / output.position.w;
	output.normal = norm;
	output.diffuse = input.diffuse;
	output.specular = input.specular;
	output.shininess = input.shininess;
	output.ambient = input.ambient;
	output.uv = input.uv;
	output.toonIndex = input.toonIndex;
	output.edgeFlag = input.edgeFlag;

	return output;
}

// ----------------------------------------------------------------------------
// トゥーン計算。
float4 CalcToon(float3 norm, float4 diffuse, float3 ambient, uint toonIndex)
{
	// 法線と光源ベクトルの内積から、トゥーン用テクスチャの v を計算する。
	float3 Ldir = -light.directionInViewSpace;
	float cosT = dot(norm, Ldir);
	float toon_v = saturate((-cosT + 1.0) * 0.5);
	float4 color = float4(diffuse.rgb * light.diffuse + ambient * light.ambient, diffuse.a) * texToonArray.Sample(samp, float3(0, toon_v, toonIndex));

	return color;
}

// ----------------------------------------------------------------------------
// 深度マップ法でエッジ描画。
Texture2D depthMap;

float4 PS_Edge_Depth(PS_INPUT input) : SV_TARGET
{
	// 深度値を深度バッファに出力するだけなのでやる事無し（ピクセルシェーダなしでもいいっぽい？）
	return float4(0, 0, 0, 1);
}

bool TestDepth(float depth, float depthTarget)
{
	if (depthTarget < depth)	// depthTarget より depth が手前の場合しか計算しない（隣り合うピクセルが両方エッジと判定されない様にする）。
		return false;
	float depthThreshold = 0.001;	// 大きくすればエッジが細くなる。
	if (depthTarget - depth  < depthThreshold)	
	{
		return false;
	}
	return true;	// エッジと判定。
}

float4 PS_Toon_Edge_Depth(PS_INPUT input) : SV_TARGET
{
	// 深度値を取得する。
	uint width, height;
	depthMap.GetDimensions(width, height);
//	int x = input.position.x;
//	int y = input.position.y;
//	int mipLevel = 0;
//	float d0 = depthMap.Load(int3(x, y, mipLevel)).r;
//	float d1 = depthMap.Load(int3(clamp(x-1, 0, width), y, mipLevel)).r; 
//	float d2 = depthMap.Load(int3(clamp(x+1, 0, width), y, mipLevel)).r; 
//	float d3 = depthMap.Load(int3(x, clamp(y-1, 0, height), mipLevel)).r; 
//	float d4 = depthMap.Load(int3(x, clamp(y+1, 0, height), mipLevel)).r; 
	float perPixelX = 1.0 / width;
	float perPixelY = 1.0 / height;
	float2 xy = float2(input.position.x / width, input.position.y / height); 
	float d0 = depthMap.Sample(samp, xy).r;
	float d1 = depthMap.Sample(samp, saturate(xy + float2(-perPixelX, 0))).r; 
	float d2 = depthMap.Sample(samp, saturate(xy + float2(+perPixelX, 0))).r; 
	float d3 = depthMap.Sample(samp, saturate(xy + float2(0, -perPixelY))).r; 
	float d4 = depthMap.Sample(samp, saturate(xy + float2(0, +perPixelY))).r; 

	// どれかのピクセルとエッジ判定が出れば、エッジカラーを出力する。
	if (input.edgeFlag > 0)
	{
		if (TestDepth(d0, d1)
		|| TestDepth(d0, d2)
		|| TestDepth(d0, d3)
		|| TestDepth(d0, d4))
		{
			return float4(0, 0, 0, 1);
		}
	}

	float3 norm = normalize(input.normal);
	float4 color = CalcToon(norm, input.diffuse, input.ambient, input.toonIndex);
	float3 specular = CalcSpecular(input.positionView, norm, input.specular, input.shininess);
	return saturate(tex.Sample(samp, input.uv) * color + float4(specular, 0.0));
}

// ----------------------------------------------------------------------------
// 法線深度マップ法でエッジ描画。
Texture2D normalDepthMap;

float4 PS_Edge_NormalDepth(PS_INPUT input) : SV_TARGET
{
	float4 result;

	// 法線を RGB として出力。-1～1 の 値を 0～1 に収める。
	result.rgb = (input.normal + 1) * 0.5;

	// 深度値を A として出力。
	result.a = input.depth;

	return result;
}

bool IsZero(float3 v)
{
	return v.x == 0 && v.y == 0 && v.z == 0;
}

float3 NormalBufferValueToNormal(float3 val)
{
	return normalize(val.rgb - 0.5);
}

bool TestDepthNormal(float depth, float3 normal, float depthTarget, float3 normalTarget)
{
	if (depthTarget < depth)	// depthTarget より depth が手前の場合しか計算しない。
		return false;
	float depthThreshold = 0.001;
	if (depthTarget - depth  < depthThreshold)	
	{
		float normalThreshold = cos(radians(80));
		if (!IsZero(normalTarget) && dot(normal, NormalBufferValueToNormal(normalTarget)) >= normalThreshold)
		{
			return false;
		}
	}
	return true;	// エッジと判定。
}

float4 PS_Toon_Edge_NormalDepth(PS_INPUT input) : SV_TARGET
{
	// 法線と深度値を取得する。
	uint width, height;
	normalDepthMap.GetDimensions(width, height);
//	int x = input.position.x;
//	int y = input.position.y;
//	int mipLevel = 0;
//	float4 p0 = normalDepthMap.Load(int3(x, y, mipLevel)).r;
//	float4 p1 = normalDepthMap.Load(int3(clamp(x-1, 0, width), y, mipLevel)).r; 
//	float4 p2 = normalDepthMap.Load(int3(clamp(x+1, 0, width), y, mipLevel)).r; 
//	float4 p3 = normalDepthMap.Load(int3(x, clamp(y-1, 0, height), mipLevel)).r; 
//	float4 p4 = normalDepthMap.Load(int3(x, clamp(y+1, 0, height), mipLevel)).r; 
	float perPixelX = 1.0 / width;
	float perPixelY = 1.0 / height;
	float2 xy = float2(input.position.x / width, input.position.y / height); 
	float4 p0 = normalDepthMap.Sample(samp, xy);
	float4 p1 = normalDepthMap.Sample(samp, saturate(xy + float2(-perPixelX, 0))); 
	float4 p2 = normalDepthMap.Sample(samp, saturate(xy + float2(+perPixelX, 0))); 
	float4 p3 = normalDepthMap.Sample(samp, saturate(xy + float2(0, -perPixelY))); 
	float4 p4 = normalDepthMap.Sample(samp, saturate(xy + float2(0, +perPixelY))); 
	float d0 = p0.a;
	float d1 = p1.a;
	float d2 = p2.a;
	float d3 = p3.a;
	float d4 = p4.a;
	float3 n0 = NormalBufferValueToNormal(p0.rgb);
	float3 n1 = p1.rgb;
	float3 n2 = p2.rgb;
	float3 n3 = p3.rgb;
	float3 n4 = p4.rgb;
	
	// どれかのピクセルとエッジ判定が出れば、エッジカラーを出力する。
	if (input.edgeFlag > 0)
	{
		if (TestDepthNormal(d0, n0, d1, n1)
		|| TestDepthNormal(d0, n0, d2, n2)
		|| TestDepthNormal(d0, n0, d3, n3)
		|| TestDepthNormal(d0, n0, d4, n4)
		)
		{
			return float4(0, 0, 0, 1);
		}
	}
	float3 norm = normalize(input.normal);
	float4 color = CalcToon(norm, input.diffuse, input.ambient, input.toonIndex);
	float3 specular = CalcSpecular(input.positionView, norm, input.specular, input.shininess);
	return saturate(tex.Sample(samp, input.uv) * color + float4(specular, 0.0));
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
