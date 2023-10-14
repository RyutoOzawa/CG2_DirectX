#pragma once
//グラフィックスパイプライン
//パイプラインの生成を助けたり、ブレンド設定を変えやすくするためのクラス

#include<d3d12.h>
#include<vector>
#include<wrl.h>
#include"DirectX.h"


#define BLEND_NOBLEND	(0)	//ブレンド無し
#define BLEND_ALPHA		(1)	//αブレンド
#define BLEND_ADD		(2)	//加算ブレンド
#define BLEND_SUB		(3)	//減算ブレンド
#define BLEND_INV		(4)	//反転ブレンド

class GpPipeline
{
public:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> state;//パイプラインステート
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};//パイプラインデスク
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = desc.BlendState.RenderTarget[0];//レンダーターゲットのブレンド設定
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

public:

	/// <summary>
	/// パイプラインの各設定
	/// </summary>
	/// <param name="vsBlob">頂点シェーダオブジェクト</param>
	/// <param name="psBlob">ピクセルシェーダオブジェクト</param>
	/// <param name="inputLayout_">頂点レイアウト配列</param>
	/// <param name="blendmode">ブレンド設定(0:none 1:alpha 2:add 3:sub 4:inv)</param>
	/// <param name="primitiveNum">プリミティブ形状(0:triangle 1:line)</param>
	void SetPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_, int blendmode = BLEND_NOBLEND,int primitiveNum = 0);
	void SetPipelineState(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState_);
	void SetBlendAlpha();
	void SetBlendAdd();
	void SetBlendSub();
	void SetBlendInv();

};

