#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include <cassert>
#include "BossBullet.h"
#include<sstream>
#include"Sprite.h"
#include"Audio.h"

class BossPhase_1
{

public:// メンバ関数
	~BossPhase_1();

	// 初期化関数
	void Initialize(SpriteManager* spriteManager);

	// 更新処理
	void Update(Vector3 playerPos);

	/// <summary>
	/// 形態変化の更新
	/// </summary>
	void ChangeUpdate(float animationTimer,float animationMax);

	/// <summary>
	/// タイトルシーン〜ゲームシーンへ遷移するようの更新処理
	/// </summary>
	void TitleUpdate();

	// 描画処理
	void Draw(ViewProjection viewprojection);
	void MedamaDraw(ViewProjection viewprojection);

	// 描画処理
	void DrawUI();

	WorldTransform GetPos() { return worldTransform_[0]; }

	//衝突を検出したら呼び出されたるコールバック関数
	void OnCollision();

	std::unique_ptr<BossBullet>& GetBullet() { return bullet; }

	WorldTransform GetWorldTransformP() const { return worldTransform_[0]; }

	int GetHP() { return HP; }

	void Rset();

	void SetRotation(Vector3 rotation);

private:// 静的メンバ関数

	// ブロックを飛ばす処理
	void FlyBlocks(Vector3 playerPos);

	// ブロックをランダムに抽選して浮かせる処理
	void FloatRandomBlocks();

	// 弾を打つ関連の初期化
	void ResetFlyBlocks();

	// 行列の転送と更新
	void TransferMat();

	void TurnBodyToPlayer(Vector3 playerPos);
private:// メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_[27];
	WorldTransform medamaWorldTransform;

	// インプット
	Input* input_ = nullptr;

	//モデル
	Object3d* model_ = nullptr;
	Object3d* medamaModel = nullptr;

	//デスフラグ
	bool isDead_ = false;

	// 消すときのフラグ
	bool AnnihilationFlag[27];

	// ランダムにとるための変数
	int randomBlock = 0;

	// ブロックを浮かし終わるまでのフラグ
	bool FloatBlockFlagM = false; // 座標をマイナス
	bool FloatBlockFlagP = false; // 座標をプラス
	
	// 押し出すときの角度
	Vector3 rotaAngle;

	// ブロックを動かす前の位置
	Vector3 oldPos;

	// ボスバレット
	std::unique_ptr<BossBullet> bullet;

	// 徐々に拡大する時のスケール
	Vector3 expansionScale;

	// 徐々に拡大するときのスピード
	Vector3 expansionScaleSpeed = { 0.05f,0.05f,0.05f };

	//タイトル挙動用回転角
	float titleRadian = 0;

	const int maxHP = 10;
	int HP = maxHP;

	Sprite* spriteHP;
	Sprite* spriteHPBar;

	Sound damageSound;
};

