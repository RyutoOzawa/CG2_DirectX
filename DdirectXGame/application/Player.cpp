#include "Player.h"
#include"Input.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,100 };





}

void Player::Update()
{
	//ˆÚ“®
	Move();

	//UŒ‚
	Attack();



	//’e‚ÌXV
	for(std::unique_ptr<PlayerBullet>& bullet:bullets){
		bullet->Update();
	}

}

void Player::Draw()
{
	//©•ª‚Ì•`‰æ
	Object3d::Draw();

	//’e‚Ì•`‰æ
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}
}

void Player::Move()
{
	//À•W‚É‰ÁZ‚·‚é‘¬“x
	Vector3 spd{};
	float baseSpd = 0.5f;


	//WASDˆÚ“®
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



	//Œ»İÀ•W‚ğæ“¾
	pos = position;
	//‰ÁZ‚µ‚Ä‘ã“ü
	pos += spd;
	position = pos;

	//obj3d‚ÌXV
	Object3d::Update();

}

void Player::Attack()
{
	//ƒXƒy[ƒXƒL[‚Å’e”­Ë
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE)) {
		//’e‚Ì¶¬‚Æ‰Šú‰»
		std::unique_ptr< PlayerBullet> newBullet =  std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel, GetPosition());

		//’e‚Ì“o˜^
		bullets.push_back(std::move(newBullet));
	}


}
