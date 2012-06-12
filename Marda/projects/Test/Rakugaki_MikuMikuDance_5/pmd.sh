
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
float3 CalcDiffuse(float3 norm, float3 diffuse)
{
	// Kd : ���Z���ʁB
	// Dd : �}�e���A���̃f�B�t���[�Y���ˌW���B
	// Cd : ���C�g�̃f�B�t���[�Y���ː����B
	// N  : �@���B
	// Ldir : ���_���烉�C�g�֌������x�N�g���B
	// Kd = sum[Dd * Cd * (N�ELdir)]	; ���C�g�̐����̍��v�l�B
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
// ���C�e�B���O�����B
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
// �O�[���[�V�F�[�f�B���O�B
// �W�I���g���V�F�[�_���ȗ�����ƁADirect3D10������ɐF���Ԃ��ăO�[���[�V�F�[�f�B���O�ɂȂ�ׁA
// ���_�V�F�[�_�Ń��C�e�B���O�v�Z���s���i���ׂ̈ɂ͒��_�V�F�[�_�̓��͂ɖ@�����K�v�j�B
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
// �t���b�g�V�F�[�f�B���O�B
// �@�����W�I���g���V�F�[�_�Ōv�Z���A���̖@���Ń��C�e�B���O����B
// �g���C�A���O���̊e���_�J���[�͓����@���Ōv�Z����A�����F�ɂȂ�ׁA�t���b�g�V�F�[�f�B���O�ƂȂ�B
// �������A�f�B���N�V���i�����C�g�ȊO�̃��C�g���������ꍇ�́A�������璸�_�̋������ω�����
// �F���ς���Ă��܂��ׁA�����ʂ̏������s��Ȃ���΂Ȃ�Ȃ��B
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

	// �@���̌v�Z�Binput�͓������W�ɂȂ��Ă���̂�w�����Ŋ����Ă���v�Z�B
	float3 position0 = (input[0].position.xyz / input[0].position.w);
	float3 position1 = (input[1].position.xyz / input[1].position.w);
	float3 position2 = (input[2].position.xyz / input[2].position.w);
	float3 norm = normalize(cross(position1 - position0, position2 - position0));

	// �F�����肵�ďo�́B
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
// ���C�e�B���O�Ȃ��^�O�[���[�^�t���b�g�V�F�[�f�B���O���p�s�N�Z���V�F�[�_�B
float4 PS_Default(PS_INPUT input) : SV_TARGET
{
	return saturate(tex.Sample(samp, input.uv) * float4(input.color, 1.0) + float4(input.specular, 1.0));
}

// ----------------------------------------------------------------------------
// �t�H���V�F�[�f�B���O�B
// �s�N�Z���V�F�[�_�̓��̓p�����[�^�́A���_�^�W�I���g���V�F�[�_�̏o�̓p�����[�^���玩���I�ɕ�Ԃ��Ă����͗l�B
// �@�����o�͂��Ă����΁A�e���_�Ԃ̃s�N�Z���̖@������Ԃ��Ă����B�������A���`��ԂŐ��K������Ă��Ȃ��Ǝv����̂� normalize ����B
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
// �g�D�[���V�F�[�f�B���O�B
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
	// �@���ƌ����x�N�g���̓��ς���A�g�D�[���p�e�N�X�`���� v ���v�Z����B
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
