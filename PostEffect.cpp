#include "PostEffect.h"

PostEffect::PostEffect() :Sprite(
	100,			//テクスチャ番号
	{0,0},			//座標
	{500.0f,500.0f},//サイズ
	{1,1,1,1},		//色
	{0.0f,0.0f},	//アンカーポイント
	false,			//左右反転フラグ
	false			//上下反転フラグ
)
{

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//非表示なら処理終了
	if (isInvisible) {
		return;
	}

	size = { 500,500 };

	Update();

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャの設定コマンド
	SetTextureCommand(textureIndex);

	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//描画コマンド
	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}
