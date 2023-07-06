#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,50 };

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a���������������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, radius, 0)));



}

void Player::Update()
{
	//����ł�e������
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});


	//�ړ�
	Move();

	//�U��
	Attack();



	//�e�̍X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
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

void Player::OnCollision(const CollisionInfo& info)
{

	static int a = 0;
	a++;

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

		//�e�̑��x
		const float bulletSpdBase = 1.0f;
		Vector3 velocity(0, 0, bulletSpdBase);

		//�e�̐����Ə�����
		std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel, GetPosition(), velocity);

		//�e�̓o�^
		bullets.push_back(std::move(newBullet));
	}


}
