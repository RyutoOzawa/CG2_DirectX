#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include"ParticleManager.h"

//3Dオブジェクトなのでobj3dクラスを継承
class Player :public Object3d
{
public:	//メンバ関数

	//初期化
	void Initialize();

	//更新
	void Update();

	void Draw();

	void DrawParticle();

	void SetBulletModel(Model* model) { bulletModel = model; }

	Vector3 GetPosition()const { return { matWorld.m[3][0],matWorld.m[3][1] ,matWorld.m[3][2] }; }

	void OnCollision(const CollisionInfo& info) override;

private://メンバ変数

	Vector3 pos;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	//弾のモデルデータ
	Model* bulletModel = nullptr;

	//ヒットパーティクル
	ParticleManager hitParticle;

	Object3d reticleObj;


private://内部処理用メンバ関数

	//上下左右移動
	void Move();

	//攻撃
	void Attack();

};

