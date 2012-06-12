
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
// ���_�Ɩ@���ɃX�L�j���O�ƃ��[���h�r���[�ϊ����s���B
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
// �t�H�����˃��f���Ɋ�Â����C�e�B���O�����B�߂�ǂ������̂łƂ肠�����f�B���N�V���i�����C�g��̂݁B
//float3 CalcDiffuse(float3 norm, float3 diffuse)
//{
//	// Kd : ���Z���ʁB
//	// Dd : �}�e���A���̃f�B�t���[�Y���ˌW���B
//	// Cd : ���C�g�̃f�B�t���[�Y���ː����B
//	// N  : �@���B
//	// Ldir : ���_���烉�C�g�֌������x�N�g���B
//	// Kd = sum[Dd * Cd * (N�ELdir)]	; ���C�g�̐����̍��v�l�B
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
	// Ks : ���Z���ʁB
	// Ds : �}�e���A���̃X�y�L�������ˌW���B
	// Cs : ���C�g�̃X�y�L�������ː����B
	// N  : �@���B
	// Ldir : ���_���烉�C�g�֌������x�N�g���B
	// R  : ���ˌ��̔��˃x�N�g���B
	// V  : ���_���王���֌������x�N�g���B
	// a  : ���ʔ��ˎw���Bshininess
	// Ks = sum[Ds * Cs * max(R�EV, 0)^a * (N�ELdir > 0 ? 1 : 0)]
	float3 Ldir = -light.directionInViewSpace;
	float3 R = reflect(light.directionInViewSpace, norm);
	float3 V = normalize(float3(0, 0, 0) - pos);

	shininess += 0.0001;	
		// max(dot()), shininess ���� 0 ���ƌ��ʂ��񐔂ɂȂ��Ă��܂��ׁA���������l�𑫂��� shininess �� 0 �ɂȂ�Ȃ��l�ɂ��Ă����B
		// ��pow(0, 0)�͏����n�ˑ��炵���B�������萔���ƁA�R���p�C������ 1 �ɒu���������Ă��܂��l�Ȃ̂ŁA�������ϐ��̎��ƌ��ʂ��قȂ�B������ɂ��Ă������Ȃ���΂Ȃ�Ȃ��B
		// ����shininess ���́A�����ڂŒl�����߂Ă������̂�����Ƃ̎��iMJ����k�j�B
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
// ���ʒ��_�V�F�[�_�B
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
// �g�D�[���v�Z�B
float4 CalcToon(float3 norm, float4 diffuse, float3 ambient, uint toonIndex)
{
	// �@���ƌ����x�N�g���̓��ς���A�g�D�[���p�e�N�X�`���� v ���v�Z����B
	float3 Ldir = -light.directionInViewSpace;
	float cosT = dot(norm, Ldir);
	float toon_v = saturate((-cosT + 1.0) * 0.5);
	float4 color = float4(diffuse.rgb * light.diffuse + ambient * light.ambient, diffuse.a) * texToonArray.Sample(samp, float3(0, toon_v, toonIndex));

	return color;
}

// ----------------------------------------------------------------------------
// �[�x�}�b�v�@�ŃG�b�W�`��B
Texture2D depthMap;

float4 PS_Edge_Depth(PS_INPUT input) : SV_TARGET
{
	// �[�x�l��[�x�o�b�t�@�ɏo�͂��邾���Ȃ̂ł�鎖�����i�s�N�Z���V�F�[�_�Ȃ��ł��������ۂ��H�j
	return float4(0, 0, 0, 1);
}

bool TestDepth(float depth, float depthTarget)
{
	if (depthTarget < depth)	// depthTarget ��� depth ����O�̏ꍇ�����v�Z���Ȃ��i�ׂ荇���s�N�Z���������G�b�W�Ɣ��肳��Ȃ��l�ɂ���j�B
		return false;
	float depthThreshold = 0.001;	// �傫������΃G�b�W���ׂ��Ȃ�B
	if (depthTarget - depth  < depthThreshold)	
	{
		return false;
	}
	return true;	// �G�b�W�Ɣ���B
}

float4 PS_Toon_Edge_Depth(PS_INPUT input) : SV_TARGET
{
	// �[�x�l���擾����B
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

	// �ǂꂩ�̃s�N�Z���ƃG�b�W���肪�o��΁A�G�b�W�J���[���o�͂���B
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
// �@���[�x�}�b�v�@�ŃG�b�W�`��B
Texture2D normalDepthMap;

float4 PS_Edge_NormalDepth(PS_INPUT input) : SV_TARGET
{
	float4 result;

	// �@���� RGB �Ƃ��ďo�́B-1�`1 �� �l�� 0�`1 �Ɏ��߂�B
	result.rgb = (input.normal + 1) * 0.5;

	// �[�x�l�� A �Ƃ��ďo�́B
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
	if (depthTarget < depth)	// depthTarget ��� depth ����O�̏ꍇ�����v�Z���Ȃ��B
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
	return true;	// �G�b�W�Ɣ���B
}

float4 PS_Toon_Edge_NormalDepth(PS_INPUT input) : SV_TARGET
{
	// �@���Ɛ[�x�l���擾����B
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
	
	// �ǂꂩ�̃s�N�Z���ƃG�b�W���肪�o��΁A�G�b�W�J���[���o�͂���B
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
// �{�[���`��p�B
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
