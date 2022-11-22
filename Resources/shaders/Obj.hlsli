cbuffer WorldTransform : register(b0)
{
	matrix mat; // ３Ｄ変換行列
};

cbuffer Material : register(b1)
{
	float3 m_ambient : packoffset(c0);//アンビエント係数
	float3 m_diffuse : packoffset(c1);//ディフューズ係数
	float3 m_specular : packoffset(c2);//スペキュラー係数
	float m_alpha : packoffset(c2.w);//アルファ
}

cbuffer viewProjection : register(b2) 
{
	matrix view;
	matrix projection;
	float3 cameraPos;
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	//float4 worldpos : POSITION;	//ワールド座標
	float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
};
