#include "Obj.hlsli"

//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(mul(projection, view), world), pos);
	output.svpos = pos;
	//output.normal = normal;
	//output.uv = uv;
	return output;
}