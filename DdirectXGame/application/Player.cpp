#include "Player.h"
#include"Input.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,100 };

}

void Player::Update()
{

	Move();


	//obj3d�̍X�V
	Object3d::Update();
}

void Player::Move()
{
	//���W�ɉ��Z���鑬�x
	Vector3 spd{};
	float baseSpd = 0.5f;


	//WASD�ړ�
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

	//���ݍ��W���擾
	pos = position;
	
	//���Z���đ��
	pos += spd;
	position = pos;
}
