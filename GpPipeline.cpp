#include "GpPipeline.h"



void GpPipeline::SetPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, int blendmode)
{
	inputLayout.push_back(
		{
		"POSITION",										//セマンティック名
		0,												//同じセマンティック名が複数ある時に使うインデックス（0でよい）
		DXGI_FORMAT_R32G32B32_FLOAT,					//要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
		0,												//入力スロットインデックス(0でよい)
		D3D12_APPEND_ALIGNED_ELEMENT,					//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//入力データ種別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0												//一度に描画するインスタンス数(0でよい)
		});

	inputLayout.push_back(
		{//法線ベクトル
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});
	inputLayout.push_back({//uv座標
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});

	//デプスステンシルステートの作成
	desc.DepthStencilState.DepthEnable = true;	//深度テストを行う
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
	desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//小さければ合格
	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット

	// シェーダーの設定
	desc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	desc.VS.BytecodeLength = vsBlob->GetBufferSize();
	desc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	desc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
	desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	desc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// レンダーターゲットのブレンド設定
	//blenddesc = desc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画

	//共通設定（アルファ値）
	blenddesc.BlendEnable = true;					//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を  0%使う


	//
	//引数でブレンド設定を変更
	switch (blendmode) {
	case BLEND_NOBLEND: {
		//ブレンド設定のフラグを下す
		blenddesc.BlendEnable = false;
		break;
	}
	case BLEND_ALPHA: {
		SetBlendAlpha();
		break;
	}
	case BLEND_ADD: {
		SetBlendAdd();
		break;
	}
	case BLEND_SUB: {
		SetBlendSub();
		break;
	}
	case BLEND_INV: {
		SetBlendInv();
		break;
	}

	default: {
		break;
	}
	}

	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-デストカラーの色
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;				//使わない

	//半透明合成（アルファブレンド）
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値

	// 頂点レイアウトの設定
	desc.InputLayout.pInputElementDescs = inputLayout.data();
	desc.InputLayout.NumElements = inputLayout.size();

	// 図形の形状設定
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	desc.NumRenderTargets = 1; // 描画対象は1つ
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	desc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング


}

void GpPipeline::SetBlendAlpha()
{
	//半透明合成（アルファブレンド）
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソースのアルファ値
}

void GpPipeline::SetBlendAdd()
{
	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う
}

void GpPipeline::SetBlendSub()
{
	//減算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;				//デストの値を100%使う

}

void GpPipeline::SetBlendInv()
{
	//色反転
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-デストカラーの色
	blenddesc.DestBlend = D3D12_BLEND_ZERO;				//使わない
}
