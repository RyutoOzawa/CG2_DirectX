cbuffer cbuff0 : register(b0)
{
	matrix world;//3D行列
	float4 color;//色
}

cbuffer cbuff1 : register(b1)
{
	matrix view;//ビュー行列とプロジェクション行列
	matrix projection;//ビュー行列とプロジェクション行列
}

//頂点シェーダからピクセルシェーダへのやり取りに使う構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;//システム用頂点座標
};