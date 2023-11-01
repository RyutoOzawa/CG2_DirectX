cbuffer cbuff0 : register(b0)
{
	matrix mat;//ビルボード変換行列
	float4 color;//色
}

cbuffer cbuff1 : register(b1)
{
	matrix view;//カメラ行列
	matrix projection;
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : SV_POSITION; // 頂点座標
	float scale : TEXCOORD;//スケール
};

//ジオメトリシェーダーからピクセルシェーダへの出力
struct GSOutput {
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :TEXCOORD; // uv値
};