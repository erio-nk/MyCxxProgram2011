/**
	�K���V�F�[�_
 */
 
// �g�����X�t�H�[���s��
float4x4 mWorld;
float4x4 mView;
float4x4 mProj;

// ���C�e�B���O���
float3 vLightDir;
float4 vLightColor;

// �f�B�t���[�Y
float4 vDiffuse;

// ���_�V�F�[�_���̓p�����[�^
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
};

// �s�N�Z���V�F�[�_���̓p�����[�^
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

// ���_�V�F�[�_
PS_INPUT VS(VS_INPUT vsIn)
{
	PS_INPUT psIn;
	
	// ���_���g�����X�t�H�[������
	psIn.Pos = mul(vsIn.Pos, mWorld);
	psIn.Pos = mul(psIn.Pos, mView);
	psIn.Pos = mul(psIn.Pos, mProj);
	
	// ���C�e�B���O�i�f�B�t���[�Y�̂݁j
	//psIn.Color = vsIn.Color * vLightColor * saturate(dot(vsIn.Normal, vLightDir));
	psIn.Color = vDiffuse * vLightColor * saturate(dot(vsIn.Normal, vLightDir));
	
	return psIn;
}

// �s�N�Z���V�F�[�_
float4 PS(PS_INPUT psIn) : SV_Target
{
	return psIn.Color;
}

// �e�N�j�b�N
technique10 Render
{
	// �p�X�i���ĉ��H�j
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

