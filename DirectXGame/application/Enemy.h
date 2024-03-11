#pragma once
//敵クラス
//ゲームに登場する敵単体の処理を行う

#include"Object3d.h"
#include"SplineCurve.h"
#include"ParticleManager.h"
#include"EnemyBullet.h"


class Enemy : public Object3d
{
public:

	//パーティクル更新
	static void EnemyParticleUpdate();

	//敵の静的初期化
	static void EnemyInitialize(TextureData* texData, Model* enemyModel,Model* bulletModel_);

	//パーティクル描画
	static void DrawParticle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="points">移動用スプライン曲線の座標配列</param>
	void Initialize(std::vector<Vector3>& points);

	/// <summary>
	/// 初期化(ファイル読み込み版)
	/// </summary>
	/// <param name="spawnPos">座標</param>
	/// <param name="leaveTime">退避までのフレーム</param>
	void Initialive(const Vector3& spawnPos, uint16_t leaveTime_);


	//更新
	void Update(const Vector3& playerWorldPos,const Matrix4& cameraMat);

	//描画
	void Draw();

	//スポーン
	void Spawn();
	//当たり判定コールバック
	void OnCollision([[maybe_unused]] const CollisionInfo& info) override;
	//生存フラグ
	bool IsAlive()const { return isAlive; }
	//攻撃
	void Attack(const Vector3& playerWorldPos);
	//死亡
	void Death();

private:

	static ParticleManager particleManager;

	const float enemyColSize = 9.0f;

	//動線
	SplineCurve moveLine;
	uint16_t leaveTime = 0;
	Vector3 stayPosition = {};
	Vector3 leaveSpd = {};
	const float leaveSpdBase = 5.0f;

	bool isAlive = false;

	//弾関係
	INT32 shotInterval = 0;
	static const INT32 shotCoolTime;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	static Model* model;
	static Model* bulletModel;

	//移動
	void Move(const Matrix4& camMat,const Vector3& camPos);

	//退避
	void Leave();

	//大きさの制御
	void ScaleControll();

	//スピン
	void Spin();

	//攻撃時回転のイージングデータ
	Easing::EasingData easeAtkRot;

	Vector3 baseScale = { 3,3,3 };
};

