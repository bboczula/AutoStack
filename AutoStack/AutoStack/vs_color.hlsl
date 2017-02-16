struct VertexShaderInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.position = float4((input.position.x - 400.0f) / 400.0f, (input.position.y - 300.0f) / 300.0f, input.position.z, 1.0f);
	output.color = float4(input.color.r, input.color.g, input.color.b, 1.0f);
	return output;
}