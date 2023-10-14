#pragma once
//スプライト
//2Dの画像を描画するための情報を持つクラス

#include<DirectXMath.h>
#include"Texture.h"
#include<string>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"

enum VertexNumber {
	LB,	//左下
	LT,	//左上
	RB,	//右下
	RT,	//右上
};

class Sprite
{
public://サブクラス
	struct VertexPosUv {
		Vector3 pos;	//xyz座標
		Vector2 uv;	//uv座標
	};

	struct ConstBufferData {
		Vector4 color;
		Matrix4 mat;
	};

protected://静的メンバ変数

	static std::string defaultTextureDirectoryPath;//デフォルトのディレクトリパス
	static ID3D12Device* device;//デバイス
	static ID3D12GraphicsCommandList* cmdList;//コマンドリスト	
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;		//パイプラインステート
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;		//ルートシグネチャ

	static Matrix4 matProjection;	//射影行列

public://静的メンバ関数
	static void StaticInitialize(ID3D12Device* device_, ID3D12GraphicsCommandList* cmdList_, int windowWidth, int windowHeight);

	static void BeginDraw();



	//スプライト用パイプラインステートとルートシグネチャの生成
	static void CreatePipeline2D();

protected:	//メンバ変数

	//マネージャクラスのポインタ
	VertexPosUv vertices[4] = {};		//頂点座標データ
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff = nullptr;	//定数バッファ
	ConstBufferData* constMap = nullptr;//定数バッファ構造体
	TextureData* texData = nullptr;

	Matrix4 matWorld{};	//ワールド変換行列
	float rotation = 0.0f;	//回転角
	Vector2 position = { 0.0f,0.0f };	//座標
	Vector4 color = { 1,1,1,1 };	//座標
	Vector2 size = { 100.0f,100.0f };//スプライトサイズ
	Vector2 anchorPoint = { 0.0f,0.0f };//アンカーポイント(座標変換の基準点)
	bool isFlipX = false;	//左右反転フラグ
	bool isFlipY = false;	//上下反転フラグ
	bool isInvisible = false;	//非表示フラグ

	Vector2 textureLeftTop = { 0.0f,0.0f };	//テクスチャ左上座標
	Vector2 textureSize = { 100.0f,100.0f };	//テクスチャ切り出しサイズ

public: //メンバ関数

	Sprite();
	Sprite(TextureData* texData,Vector2 pos,Vector2 size_,Vector4 color_,Vector2 anchorP,bool isFlipX_,bool isFlipY_);

	//初期化
	void Initialize(std::string filename);
	void Initialize(TextureData* texData_);

	//描画
	void Draw();

	//setter
	void SetColor(const Vector4& color_) { color = color_; }
	void SetPos(const Vector2& pos) { position = pos; }
	void SetRotation(float rotation_) { rotation = rotation_; }
	void SetSize(const Vector2& size_) { size = size_; }
	void SetAnchorPoint(const Vector2& point) { anchorPoint = point; }
	void SetFlipX(bool flipX) { isFlipX = flipX; }
	void SetFlipY(bool flipY) { isFlipY = flipY; }
	void SetInvisible(bool flag) { isInvisible = flag; }
	void SetTexture(TextureData* data) { texData = data; }
	void SetTextureLeftTop(const Vector2& leftTop) { textureLeftTop = leftTop; }
	void SetTextureSize(const Vector2& size_) { textureSize = size_; }

	//getter
	const Vector2& GetPosition()const { return position; }
	float GetRotation()const { return rotation; }
	const Vector4 GetColor()const { return color; }
	const Vector2 GetSize()const { return size; }
	const Vector2 GetAnchorPoint()const { return anchorPoint; }
	bool GetIsFlipX()const { return isFlipX; }
	bool GetIsFlipY()const { return isFlipY; }
	bool GetIsInvisible()const { return isInvisible; }
	const Vector2 GetTextureLeftTop()const { return textureLeftTop; }
	const Vector2 GetTextureSize()const { return textureSize; }

	//更新
	void Update();

private:
	//テクスチャサイズをイメージサイズに合わせる
	void AdjustTextureSize();

	//テクスチャコマンドセット
	void SetTextureCommand();
};

