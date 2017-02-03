float4 main(float3 position : POSITION) : SV_POSITION
{
	// Be careful, this last parameter can disable rendering!!!
	return float4(position.xyz, 1.0f);
}