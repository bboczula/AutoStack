Texture2D shaderTexture;
SamplerState sampleType;

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return shaderTexture.Sample(sampleType, input.texCoord);
}