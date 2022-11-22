#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float4 worldNormal = normalize(mul(world, float4(normal, 0)));
	float4 worldPos = mul(world, pos);

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(mul(projection,view), world) , pos);

	output.worldpos = worldPos;
	output.normal = worldNormal.xyz;
	output.uv = uv;
	return output;
}