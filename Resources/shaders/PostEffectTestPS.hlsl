#include"PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);		//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);				//0番スロットに設定されたサンプラー

//ガウシアンブラー
float Gaussian(float2 drawUV, float2 pickUV, float sigma) {
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	float totalWeight = 0,sigma = 0.008,stepWidth = 0.001;
	float4 result = float4(0, 0, 0, 0);
	
	for (float y = -sigma * 2; y < sigma * 2; y += stepWidth) {
		for (float x = -sigma * 2; x < sigma * 2; x += stepWidth) {
			float2 pickUV = input.uv + float2(x, y);
			float weight = Gaussian(input.uv, pickUV, sigma);
			result += tex.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	result.rgb = result.rgb / totalWeight;
	result= tex.Sample(smp, input.uv);
	return float4(result.rgb,1);


}


//
//float4 main(VSOutput input) : SV_TARGET
//{
//	return float4(input.uv,0,1);
//}