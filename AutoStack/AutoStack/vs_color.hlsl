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
	// Przetlumacz ta pozycje na pozycje na GPU
	//float x = (float)(anchor.x - 400.0f) / 400.0f;
	//float y = (float)(anchor.y - 300.0f) / 300.0f;

	// Be careful, this last parameter can disable rendering!!!
	//return float4(position.xyz, 1.0f);
	//return float4((position.x - 400.0f) / 400.0f, (position.y - 300.0f) / 300.0f, position.z, 1.0f);

	VertexShaderOutput output;
	output.position = float4((input.position.x - 400.0f) / 400.0f, (input.position.y - 300.0f) / 300.0f, input.position.z, 1.0f);
	output.color = float4(0.71f, 0.13f, 0.52f, 1.0f);
	return output;
}