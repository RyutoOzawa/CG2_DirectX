#include "Player.h"
#include"Input.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,100 };





}

void Player::Update()
{
	//�ړ�
	Move();

	//�U��
	Attack();



	//�e�̍X�V
	for(std::unique_ptr<PlayerBullet>& bullet:bullets){
		bullet->Update();
	}

}

void Player::Draw()
{
	//�����̕`��
	Object3d::Draw();

	//�e�̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}
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

	//obj3d�̍X�V
	Object3d::Update();

}

void Player::Attack()
{
	//�X�y�[�X�L�[�Œe����
	if (Input::GetInstance()->IsKeyTrigger(DIK_SPACE)) {
		//�e�̐����Ə�����
		std::unique_ptr< PlayerBullet> newBullet =  std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel, GetPosition());

		//�e�̓o�^
		bullets.push_back(std::move(newBullet));
	}


}
