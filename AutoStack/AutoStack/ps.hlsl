cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float r;
	float g;
	float b;
}

float4 main(float4 position : SV_POSITION) : SV_TARGET
{
	return float4(r, g, b, 1.0f);
}