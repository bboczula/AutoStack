float4 main(float3 position : POSITION) : SV_POSITION
{
	// Przetlumacz ta pozycje na pozycje na GPU
	//float x = (float)(anchor.x - 400.0f) / 400.0f;
	//float y = (float)(anchor.y - 300.0f) / 300.0f;

	// Be careful, this last parameter can disable rendering!!!
	//return float4(position.xyz, 1.0f);
	return float4((position.x - 400.0f) / 400.0f, (position.y - 300.0f) / 300.0f, position.z, 1.0f);
}