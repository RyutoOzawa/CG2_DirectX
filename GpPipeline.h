#pragma once
#include<d3d12.h>
#include<vector>

#define BLEND_NOBLEND	(0)	//ブレンド無し
#define BLEND_ALPHA		(1)	//αブレンド
#define BLEND_ADD		(2)	//加算ブレンド
#define BLEND_SUB		(3)	//減算ブレンド
#define BLEND_INV		(4)	//反転ブレンド

class GpPipeline
{
public:

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};//パイプラインデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = desc.BlendState.RenderTarget[0];//レンダーターゲットのブレンド設定
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

public: 
	void SetPipeline(ID3DBlob* vsBlob,ID3DBlob* psBlob, int blendmode = BLEND_NOBLEND);
	void SetBlendAlpha();
	void SetBlendAdd();
	void SetBlendSub();
	void SetBlendInv();

};

