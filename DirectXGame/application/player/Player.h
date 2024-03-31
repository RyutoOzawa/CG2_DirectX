#pragma once

//プレイヤークラス
//ユーザーが操作するプレイヤー全般の処理はここで行う

#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include"ParticleManager.h"
#include"Sprite.h"
#include"EasingData.h"

class Enemy;

enum class PlayerPhase {
	Spawn,
	Alive,
	Death,
	Leave,
};

//3Dオブジェクトなのでobj3dクラスを継承
class Player :public Object3d
{
public:	//メンバ関数

	//初期化
	void Initialize(Model* model_,TextureData* reticleTexture,TextureData* healthTexture);

	//自機生成処理
	void Spawn();

	//更新
	void Update(std::list<std::unique_ptr<Enemy>>* enemys);

	//3Dモデル描画
	void Draw();

	//パーティクル描画
	void DrawParticle();

	//UI(2D)描画
	void DrawUI();

	//当たり判定コールバック
	void OnCollision([[maybe_unused]] const CollisionInfo& info) override;

	//生存判定
	bool IsAlive()const { return isAlive; }
	//getter
	INT32 GetSpawnTimer() const { return spawnTimer; }
	float GetDistanceCam()const { return distanceCamera; }

	//自機脱出処理
	void Leave();

private://メンバ変数

	//カメラからの自機の距離
	const float distanceCamera = 50.0f;

	//ローカル座標
	Vector3 localPos;

	//画面上のレティクル座標
	Vector2 reticlePosScreen{WindowsAPI::winW/2.0f,WindowsAPI::winH/2.0f};
	float reticleRadius = 32.0f;

	//弾関連
	INT32 shotInterval = 0;
	const INT32 shotCooltime = 15;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	std::unique_ptr<Model> bulletModel = nullptr;

	//ヒットパーティクル
	std::unique_ptr<ParticleManager> hitParticle = nullptr;

	//レティクル関連
	std::unique_ptr<Object3d> reticleObj = nullptr;
	std::unique_ptr<Sprite> reticleSprite = nullptr;
	Vector4 reticleColor = { 1,1,1,1 };

	//生成演出用の物
	static const INT32 haloMax = 8;
	std::unique_ptr<Model> haloModel = nullptr;
	//TODO:光輪関係を1つの構造体にまとめる
	std::array<std::unique_ptr<Object3d>,haloMax> haloObjects;
	std::array<float, haloMax> haloAlphaVel;
	std::array<float, haloMax> haloScaleVel;
	std::array<Vector3, haloMax> haloRotaVel;
	INT32 spawnTimer = 0;
	INT32 spawnTimerMax = 120;
	Easing::EasingData eDataPlayerScale;

	//脱出演出関係	
	static const uint16_t leaveHaloMax = 5;
	uint16_t leaveTimer = 0;
	const uint16_t leaveTimerMax = 120;
	float haloScale[leaveHaloMax] = { 0,0,0,0,0 };

	//HP関連
	std::unique_ptr<Sprite> healthSprite = nullptr;
	const int healthMax = 2;
	int health = healthMax;
	int healthWidthMax = 0;
	int healthWidth = healthWidthMax;
	const int damageCooltime = 120;
	int damageInterval = damageCooltime;
	const int deathCountMax = 180;
	int deathCount = deathCountMax;
	bool isAlive = false;
	bool isSpawn = false;
	//自機のフェーズ
	PlayerPhase phase = PlayerPhase::Spawn;



	Easing::EasingData easeUIAlpha;
	float UIAlpha = 0.0f;
	std::unique_ptr<Sprite> damageSprite = nullptr;


private://内部処理用メンバ関数

	//上下左右移動
	void Move();

	//攻撃
	void Attack();

	//レティクルの更新
	void ReticleUpdate(std::list<std::unique_ptr<Enemy>>* enemys);

	//HPバーの更新
	void HealthUpdate();

	//ダメージを受けた時の処理
	void Damage();

	//死んだ瞬間の処理
	void Death();

	//死亡更新処理
	void UpdateDeath();

	//生成更新処理
	void UpdateSpawn();

	//脱出更新処理
	void UpdateLeave();

};

