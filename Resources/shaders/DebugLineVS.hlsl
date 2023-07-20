#include"DebugLine.hlsli"

VSOutput main(float4 pos : POSITION) 
{
	VSOutput output;
//	output.svpos = mul(mul(viewProjection, world), pos);
	output.svpos = mul(mul(projection,view), pos);

	return output;
}