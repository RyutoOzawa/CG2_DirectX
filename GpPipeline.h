#pragma once
#include<d3d12.h>
#include<vector>

class GpPipeline
{
public:

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};//パイプラインデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = desc.BlendState.RenderTarget[0];//レンダーターゲットのブレンド設定
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

public: 
	void Initialize(ID3DBlob* vsBlob,ID3DBlob* psBlob);
	//void SetBlend();


};

