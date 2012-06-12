
// 定数バッファのデータ定義①　変更しないデータ
cbuffer cbNeverChanges : register(b0) { // 常にスロット「0」を使う
    matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer cbChangesEveryFrame : register(b1) { // 常にスロット「1」を使う
    matrix View;   // ビュー変換行列
    float3 Light;  // 光源座標(ビュー座標系)
};


// 定数バッファのデータ定義③　変更頻度の高いデータ
cbuffer cbChangesEveryObject : register(b2) { // 常にスロット「2」を使う
    matrix World;      // ワールド変換行列
};

const float4 color = {1.0, 1.0, 0.0, 1.0};	// 初期化子は反映されない模様。


// 頂点シェーダの入力データ定義
struct VS_INPUT {
    float3 Pos : POSITION;   // 頂点座標(モデル座標系)
    float3 Col : COLOR;      // 頂点色
};

// ジオメトリ シェーダの入力データ定義
struct GS_INPUT {
    float4 Pos  : SV_POSITION;  // 頂点座標(ビュー座標系)
    float4 Col  : COLOR;        // 頂点色
};

// ピクセル シェーダの入力データ定義
struct PS_INPUT {
    float4 Pos  : SV_POSITION; // 頂点座標(透視座標系)
    float3 PosView  : POSVIEW; // 頂点座標(ビュー座標系)
    float3 Norm : NORMAL;      // 法線ベクトル(ワールド座標系)
    float4 Col  : COLOR;       // 頂点色
};

// 頂点シェーダの関数
GS_INPUT VS(VS_INPUT input) {
    GS_INPUT output;
    // 頂点座標　モデル座標系→ビュー座標系
    float4 pos4 = float4(input.Pos, 1.0);
    //float4 pos4 = float4(input.Pos, 1.0 * hoge);
    output.Pos = mul(pos4, World);
    output.Pos = mul(output.Pos, View);
    // 頂点色
//    output.Col = float4(input.Col, 1.0);
    output.Col = color;
    //output.Col = float4(1.0,1.0,0.0,1.0);
    // 出力
    return output;
}

// ジオメトリ シェーダの関数
[maxvertexcount(3)]
void GS(triangle GS_INPUT input[3],
        inout TriangleStream<PS_INPUT> TriStream) {
    PS_INPUT output;
    // 法線ベクトルの計算
    float3 faceEdge  = input[0].Pos.xyz / input[0].Pos.w;
    float3 faceEdgeA = (input[1].Pos.xyz / input[1].Pos.w) - faceEdge;
    float3 faceEdgeB = (input[2].Pos.xyz / input[2].Pos.w) - faceEdge;
    output.Norm = normalize(cross(faceEdgeA, faceEdgeB));
    // 各頂点の計算
    for (int i=0; i<3; ++i) {
        // 頂点座標　ビュー座標系
        output.PosView = input[i].Pos / input[i].Pos.w;
        // 頂点座標　ビュー座標系→射影座標系
        output.Pos = mul(input[i].Pos, Projection);
        // 頂点色
        output.Col = input[i].Col;
        // 出力
        TriStream.Append(output);
    }
    TriStream.RestartStrip();
}

// ピクセル シェーダの関数
float4 PS(PS_INPUT input) : SV_TARGET
{
    // 光源ベクトル
    float3 light = Light - input.PosView;
    // 距離
    float  len = length(light);
    // 明るさ
    float bright = 30 * dot(normalize(light), normalize(input.Norm)) / pow(len, 2);
    // 色
    return saturate(bright * input.Col);
}
