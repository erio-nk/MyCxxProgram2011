
matrix Proj;
matrix View;
matrix World;
float4 color;

struct VS_INPUT 
{
	float3 position	: POSITION;
};

//struct GS_INPUT 
//{
//	float4 position	: SV_POSITION;
//};

struct PS_INPUT
{
	float4 position	: SV_POSITION;
};

PS_INPUT VS(VS_INPUT input) 
{
	PS_INPUT output;
	float4 pos4 = float4(input.position, 1.0);
	output.position = mul(pos4, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Proj);
    return output;
}

//GS_INPUT VS(VS_INPUT input) 
//{
//	GS_INPUT output;
//	float4 pos4 = float4(input.position, 1.0);
//	output.position = mul(pos4, World);
//	output.position = mul(output.position, View);
//	output.position = mul(output.position, Proj);
//    return output;
//}
//
//[maxvertexcount(3)]
//void GS(triangle GS_INPUT input[3], inout TriangleStream<PS_INPUT> TriStream) 
//{
//	PS_INPUT output;
//
////	output.position = mul(input[0].position, Proj);
//	output.position = input[0].position;
//	TriStream.Append(output);
//
////	output.position = mul(input[1].position, Proj);
//	output.position = input[1].position;
//	TriStream.Append(output);
//
////	output.position = mul(input[2].position, Proj);
//	output.position = input[2].position;
//	TriStream.Append(output);
//
//	TriStream.RestartStrip();
//}

float4 PS(PS_INPUT input) : SV_TARGET
{
    return color;
}
