#include"PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);		//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);		//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);				//0番スロットに設定されたサンプラー

//ガウシアンブラー
float Gaussian(float2 drawUV, float2 pickUV, float sigma) {
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 / sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	//色反転
	float4 colortex0 = tex0.Sample(smp,input.uv);
	colortex0 = 1-colortex0;

	//ガウシアンブラー
	float4 colortex1 = float4(0, 0, 0, 0);

	float totalWeight = 0, sigma = 0.008, stepWidth = 0.001;

	for (float y = -sigma * 2; y < sigma * 2; y += stepWidth) {
		for (float x = -sigma * 2; x < sigma * 2; x += stepWidth) {
			float2 pickUV = input.uv + float2(x, y);
			float weight = Gaussian(input.uv, pickUV, sigma);
			colortex1 += tex1.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}
	colortex1.rgb = colortex1.rgb / totalWeight;


	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1) < 0.05f) {
		color = colortex1;
	}

	 

	return float4(color.rgb, 1);
}


//
//float4 main(VSOutput input) : SV_TARGET
//{
//	return float4(input.uv,0,1);
//}