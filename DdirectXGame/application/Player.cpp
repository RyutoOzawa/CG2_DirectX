#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"
#include"Util.h"

void Player::Initialize()
{
	Object3d::Initialize();

	position = { 0,0,50 };

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	//���a���������������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0),radius));

	hitParticle.Initialize(0);

	reticleObj.Initialize();

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

	hitParticle.Update();

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

void Player::DrawParticle()
{
	hitParticle.Draw();
}

void Player::OnCollision(const CollisionInfo& info)
{

	static int a = 0;
	a++;

	//�p�[�e�B�N���̑��x
	Vector3 vel = { 0,0,0 };
	Vector3 acc = { Random(-10.0f,10.0f),Random(-10.0f,10.0f) ,Random(-10.0f,10.0f) };

	
	//�p�[�e�B�N���ǉ�
	hitParticle.Add(15, info.inter, vel, acc, 3.0f, 0.0f);

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
