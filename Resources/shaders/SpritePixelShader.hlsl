#include"Sprite.hlsli"

//Texture2D<float4> tex : register(t0);		//0番スロットに設定されたテクスチャ
//SamplerState smp : register(s0);				//0番スロットに設定されたサンプラー
//
//float4 main(VSOutput input) : SV_TARGET
//{
//	return tex.Sample(smp,input.uv) * color;
//}

cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;	//色
};

float4 main() :SV_TARGET
{
	return color;
}