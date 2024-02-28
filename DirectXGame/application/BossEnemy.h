#pragma once
//ボス敵クラス
//ゲーム最後に登場するボスの処理を行う

#include"Object3d.h"
#include"Model.h"
#include<array>
#include"SplineCurve.h"
#include"BezierCurve.h"
#include"Sprite.h"
#include"EnemyBullet.h"
#include"ParticleManager.h"
#include"EventCamera.h"

//ボスの行動列挙クラス
enum class BossAct {
	Spawn,			//スポーン
	Move,			//行動
	AttackShot,		//攻撃1
	AttackLaser,	//攻撃2
	Death,			//死亡
	BossActMax,
};

//死亡演出のフェーズ遷移
enum class BossDeathPhase {
	Burst,
	Move,
	Fall,
	Dead,
};

enum class BossAtkLaserPhase {
	RockOn,//狙う
	Charge,//チャージ
	Shot,//撃つ
	ReturnMove,//移動に戻る
};

class BossEnemy : public Object3d
{
public: 

	//初期化
	void Initialize(Model* bodyModel_,Model* barrelModel_,Object3d* parent_);

	//更新
	void Update(const Vector3& playerPos = {0.0f,0.0f,0.0f},EventCamera* eventCamera = nullptr);

	//描画
	void Draw();

	//スプライト描画
	void DrawSprite();

	//パーティクル描画
	void DrawParticle();

	//デバッグ線描画
	void DrawDebugLine();

	//終了
	void Finalize();

	//ボスのスポーン
	void Spawn(const Matrix4& cameraMatWorld,const Vector3& spawnPos = {0,0,240.0f});

	//getter
	bool IsAlive()const { return isAlive; }
	INT32 GetHealth() const { return life; }
	BossAct GetBossAct() const { return bossAct; }

private:
	Model* bodyModel;
	Model* barrelModel;
	std::unique_ptr<Model> bulletModel=nullptr;

	//カメラワーク用イベントカメラのポインタ
	EventCamera* eCamera = nullptr;

	//命関係
	bool isAlive = false;
	INT32 lifeMax = 10;
	INT32 life = lifeMax;
	TextureData* healthTexture = nullptr;
	std::unique_ptr<Sprite> healthSprite = nullptr;
	float healthWidthOneHp = 0.0f;
	uint16_t damageInterval = 0;
	uint16_t damageIntervalMax = 10;
	
	//死亡演出関係
	uint16_t explosionCountMax = 10;
	uint16_t explosionCount = explosionCountMax;
	uint16_t nextPhaseIntervalMax = 30;
	uint16_t nextPhaseInterval = nextPhaseIntervalMax;
	BossDeathPhase deathPhase = BossDeathPhase::Move;

	//砲台関係
	static const INT32 barrelMax = 4;
	const float baseBarrelDistance = 20.0f;
	std::array<Object3d, barrelMax> barrelObject;
	std::array<Vector3, barrelMax> barrelDistance;
	std::array<float, barrelMax> barrelRadian;
	Vector3 bulletOutOffset{ 0,0,0 };//モデルの弾が出る部分のオフセット
	std::array<Easing::EasingData,barrelMax> eDataBarrelMove;	//バレルの移動をイージングにするよう
	std::array<Vector3,barrelMax> movePosBeforeBarrel;	//バレルの移動前座標
	std::array<Vector3,barrelMax> movePosAfterBarrel;	//バレルの移動後座標
	Easing::EasingData eDataBarrelRot;//バレルの回転速度遷移


	//ボスの行動管理
	BossAct bossAct = BossAct::Move;
	static const INT32 bossActMax = (INT32)BossAct::BossActMax;
	std::array<INT32, bossActMax> actTime;		//各行動に使う時間
	std::array<INT32, bossActMax> moveCooltime;	//各行動の次行動までのクールタイム
	INT32 nowActTime = 0;		//現在行動の残り時間
	INT32 moveInterval = 0;		//次行動に移るまでの時間
	int count = 0;

	Sprite sp[4];

	//標的(自機)の座標
	Vector3 targetPos{ 0,0,0 };

	//移動曲線
	float lissajousTheta = 0.0f;
	float lThetaSpd = 0.5f;//リサージュ曲線用角速度
	std::vector<Vector3> curvePoints;
	float radianX = 2.0f;
	float radianY = 3.0f;
	float amplitudeX = 160.0f;
	float amplitudeY = 80.0f;

	//射撃攻撃
	std::list < std::unique_ptr<EnemyBullet>> bullets;
	static const INT32 shotPosMax = 4;
	std::array<Vector3, shotPosMax> shotPos;//射撃を行う座標配列(0:leftTop 1:leftBottom 2:rightTop 3:rightBottom)
	Easing::EasingData eDataMove;//移動用イージングデータ

	//レーザー攻撃用
	BossAtkLaserPhase laserPhase = BossAtkLaserPhase::RockOn;
	std::unique_ptr<Model> laserModel = nullptr;
	std::unique_ptr<Object3d> laserObj = nullptr;
	Vector2 rotSpdTemp = {};
	Vector2 barrelDistanceTemp = { };
	Vector2 laserScaleTemp = {};
	uint16_t laserTime = 0;

	//移動補完用座標2つ
	Vector3 movePosBefore{ 0,0,0 };
	Vector3 movePosAfter{ 0,0,0 };
	Vector3 lastPosActMove{ 0,0,0 };//移動フェーズの最後の座標
	//回転補間用行列
	Matrix4 matRotBefore;
	Matrix4 matRotAfter;

	//生成処理関係
	Vector3 spawnPosOffsetCamera;//スポーンするときのカメラからの距離(目玉)

	//パーティクルマネージャ
	std::unique_ptr<ParticleManager> particleManager = nullptr;

	//各行動の更新処理
	//スポーン更新
	void UpdateSpawn();
	//移動更新
	void UpdateMove();
	//射撃攻撃更新
	void UpdateAtkShot();
	//レーザー攻撃更新
	void UpdateAtkLaser();
	//死亡更新
	void UpdateDeath();

	//スポーン初期化
	void InitSpawn();
	//移動初期化
	void InitMove();
	//射撃攻撃初期化
	void InitAtkShot();
	//レーザー攻撃初期化
	void InitAtkLaser();
	//死亡初期化
	void InitDeath();

	//行動変化
	void ChangeAct(BossAct nextAct);

	//当たり判定コールバック
	void OnCollision([[maybe_unused]] const CollisionInfo& info)override;
	//ダメージを受ける処理
	void Damage(const Vector3& hitPos,uint16_t damage = 1);
	
	//ボススポーンのカメラワーク開始
	void StartCameraSpawn();

};

