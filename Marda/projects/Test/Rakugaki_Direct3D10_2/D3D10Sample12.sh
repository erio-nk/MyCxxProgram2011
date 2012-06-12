
// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
cbuffer cbNeverChanges : register(b0) { // ��ɃX���b�g�u0�v���g��
    matrix Projection;   // �����ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer cbChangesEveryFrame : register(b1) { // ��ɃX���b�g�u1�v���g��
    matrix View;   // �r���[�ϊ��s��
    float3 Light;  // �������W(�r���[���W�n)
};


// �萔�o�b�t�@�̃f�[�^��`�B�@�ύX�p�x�̍����f�[�^
cbuffer cbChangesEveryObject : register(b2) { // ��ɃX���b�g�u2�v���g��
    matrix World;      // ���[���h�ϊ��s��
};

const float4 color = {1.0, 1.0, 0.0, 1.0};	// �������q�͔��f����Ȃ��͗l�B


// ���_�V�F�[�_�̓��̓f�[�^��`
struct VS_INPUT {
    float3 Pos : POSITION;   // ���_���W(���f�����W�n)
    float3 Col : COLOR;      // ���_�F
};

// �W�I���g�� �V�F�[�_�̓��̓f�[�^��`
struct GS_INPUT {
    float4 Pos  : SV_POSITION;  // ���_���W(�r���[���W�n)
    float4 Col  : COLOR;        // ���_�F
};

// �s�N�Z�� �V�F�[�_�̓��̓f�[�^��`
struct PS_INPUT {
    float4 Pos  : SV_POSITION; // ���_���W(�������W�n)
    float3 PosView  : POSVIEW; // ���_���W(�r���[���W�n)
    float3 Norm : NORMAL;      // �@���x�N�g��(���[���h���W�n)
    float4 Col  : COLOR;       // ���_�F
};

// ���_�V�F�[�_�̊֐�
GS_INPUT VS(VS_INPUT input) {
    GS_INPUT output;
    // ���_���W�@���f�����W�n���r���[���W�n
    float4 pos4 = float4(input.Pos, 1.0);
    //float4 pos4 = float4(input.Pos, 1.0 * hoge);
    output.Pos = mul(pos4, World);
    output.Pos = mul(output.Pos, View);
    // ���_�F
//    output.Col = float4(input.Col, 1.0);
    output.Col = color;
    //output.Col = float4(1.0,1.0,0.0,1.0);
    // �o��
    return output;
}

// �W�I���g�� �V�F�[�_�̊֐�
[maxvertexcount(3)]
void GS(triangle GS_INPUT input[3],
        inout TriangleStream<PS_INPUT> TriStream) {
    PS_INPUT output;
    // �@���x�N�g���̌v�Z
    float3 faceEdge  = input[0].Pos.xyz / input[0].Pos.w;
    float3 faceEdgeA = (input[1].Pos.xyz / input[1].Pos.w) - faceEdge;
    float3 faceEdgeB = (input[2].Pos.xyz / input[2].Pos.w) - faceEdge;
    output.Norm = normalize(cross(faceEdgeA, faceEdgeB));
    // �e���_�̌v�Z
    for (int i=0; i<3; ++i) {
        // ���_���W�@�r���[���W�n
        output.PosView = input[i].Pos / input[i].Pos.w;
        // ���_���W�@�r���[���W�n���ˉe���W�n
        output.Pos = mul(input[i].Pos, Projection);
        // ���_�F
        output.Col = input[i].Col;
        // �o��
        TriStream.Append(output);
    }
    TriStream.RestartStrip();
}

// �s�N�Z�� �V�F�[�_�̊֐�
float4 PS(PS_INPUT input) : SV_TARGET
{
    // �����x�N�g��
    float3 light = Light - input.PosView;
    // ����
    float  len = length(light);
    // ���邳
    float bright = 30 * dot(normalize(light), normalize(input.Norm)) / pow(len, 2);
    // �F
    return saturate(bright * input.Col);
}
