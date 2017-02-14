struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(input.color.r, input.color.g, input.color.b, 1.0f);
}