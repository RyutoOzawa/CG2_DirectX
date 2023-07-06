#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,50 };

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3(0, radius, 0)));



}

void Player::Update()
{
	//死んでる弾を消す
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});


	//移動
	Move();

	//攻撃
	Attack();



	//弾の更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

}

void Player::Draw()
{
	//自分の描画
	Object3d::Draw();

	//弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Player::OnCollision(const CollisionInfo& info)
{

	static int a = 0;
	a++;

}


void Player::Move()
{
	//座標に加算する速度
	Vector3 spd{};
	float baseSpd = 0.5f;


	//WASD移動
	if (Input::GetInstance()->IsKeyPress(DIK_A)) {
		spd.x -= baseSpd;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_D)) {
		spd.x += baseSpd;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_S)) {
		spd.y -= baseSpd;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_W)) {
		spd.y += baseSpd;
	}



	//現在座標を取得
	pos = position;
	//加算して代入
	pos += spd;
	position = pos;

	//obj3dの更新
	Object3d::Update();

}

void Player::Attack()
{
	//スペースキーで弾発射
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE)) {

		//弾の速度
		const float bulletSpdBase = 1.0f;
		Vector3 velocity(0, 0, bulletSpdBase);

		//弾の生成と初期化
		std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel, GetPosition(), velocity);

		//弾の登録
		bullets.push_back(std::move(newBullet));
	}


}
