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
	if (bullet) {
		bullet->Update();
	}

}

void Player::Draw()
{
	//Ž©•ª‚Ì•`‰æ
	Object3d::Draw();

	//’e‚Ì•`‰æ
	if (bullet) {
		bullet->Draw();
	}
}

void Player::Move()
{
	//À•W‚É‰ÁŽZ‚·‚é‘¬“x
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



	//Œ»ÝÀ•W‚ðŽæ“¾
	pos = position;
	//‰ÁŽZ‚µ‚Ä‘ã“ü
	pos += spd;
	position = pos;

	//obj3d‚ÌXV
	Object3d::Update();

}

void Player::Attack()
{
	//ƒXƒy[ƒXƒL[‚Å’e”­ŽË
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE)) {
		//’e‚Ì¶¬‚Æ‰Šú‰»
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletModel, GetPosition());

		//’e‚Ì“o˜^
		bullet = newBullet;
	}


}
