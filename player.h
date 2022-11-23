#pragma once
#include "DirectX.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<cassert>
#include"playerBullet.h"
#include<memory>
#include<list>
#include<cmath>
class player
{
public:
	~player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteManager *spriteManager);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	///<summary>
	///攻撃
	///</summary>
	void Attack();

	///<summary>
	///ジャンプ
	///</summary>
	void jump();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection_);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	//衝突を検出したら呼び出されたるコールバック関数
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetworldPosition();

	const std::list<std::unique_ptr<playerBullet>>& GetBullets() { return bullets_; }

	const WorldTransform* GetWorldTransform() { return &worldTransform_; }

	void SetEndMoveRotation(Vector3 Transform);

	void AllBulletDelete() { bullets_.clear(); }

	void TransformRset(bool endOrStart);

	void Rset();

	int GetHP() { return HP; }
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Object3d* model_ = nullptr;
	Object3d* bulletModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	//弾
	std::list<std::unique_ptr<playerBullet>>bullets_;

	float PI = 3.1415927f;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;

	uint8_t jumpFlag = 0u;

	const float defGravitySpeed = 1.5;
	float gravitySpeed = defGravitySpeed;

	const int maxHP = 3;
	int HP = maxHP;

	bool isDamageInterval;

	int damageInterval;

	int attackInterval = 0;
	const int attackDelay = 30;

	Sprite* spriteHP[3];
};

