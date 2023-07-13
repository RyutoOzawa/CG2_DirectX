#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include"ParticleManager.h"
#include"Sprite.h"

class Enemy;

//3Dオブジェクトなのでobj3dクラスを継承
class Player :public Object3d
{
public:	//メンバ関数

	//初期化
	void Initialize(Model* model,uint32_t reticleTexture);

	//更新
	void Update(std::list<std::unique_ptr<Enemy>>* enemys);

	void Draw();

	void DrawParticle();

	void DrawUI();

	void SetBulletModel(Model* model) { bulletModel = model; }



	void OnCollision(const CollisionInfo& info) override;

private://メンバ変数

	//カメラからの自機の距離
	const float distanceCamera = 50.0f;

	//ローカル座標
	Vector3 pos;

	//画面上のレティクル座標
	Vector2 reticlePosScreen{WindowsAPI::winW/2.0f,WindowsAPI::winH/2.0f};
	float reticleRadius = 32.0f;

	INT32 shotInterval = 0;
	const INT32 shotCooltime = 5;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	//弾のモデルデータ
	Model* bulletModel = nullptr;

	//ヒットパーティクル
	ParticleManager hitParticle;

	Object3d reticleObj;

	Sprite reticleSprite;


private://内部処理用メンバ関数

	//上下左右移動
	void Move();

	//攻撃
	void Attack();

	//レティクルの更新
	void ReticleUpdate(std::list<std::unique_ptr<Enemy>>* enemys);


};

