struct VertexShaderInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.position = float4((input.position.x - 400.0f) / 400.0f, (input.position.y - 300.0f) / 300.0f, input.position.z, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}