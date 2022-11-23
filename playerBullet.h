#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
class playerBullet
{
	~playerBullet();
public:
	void Initialize(const Vector3& position, Vector3 Rot, const Vector3& velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されたるコールバック関数
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetworldPosition();


private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//モデル
	Object3d* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};

