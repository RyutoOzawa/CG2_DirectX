#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include"ParticleManager.h"
#include"Sprite.h"

//3Dオブジェクトなのでobj3dクラスを継承
class Player :public Object3d
{
public:	//メンバ関数

	//初期化
	void Initialize(Model* model,uint32_t reticleTexture);

	//更新
	void Update();

	void Draw();

	void DrawParticle();

	void DrawUI();

	void SetBulletModel(Model* model) { bulletModel = model; }



	void OnCollision(const CollisionInfo& info) override;

private://メンバ変数

	//ローカル座標
	Vector3 pos;

	//画面上のレティクル座標
	Vector2 reticlePosScreen{};

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
	void ReticleUpdate();

};

