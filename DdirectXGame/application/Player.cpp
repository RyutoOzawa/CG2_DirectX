#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"
#include"Util.h"

void Player::Initialize(Model* model, uint32_t reticleTexture)
{
	Object3d::Initialize();
	SetModel(model);

	position = { 0,0,50 };

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	//���a���������������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0),radius));

	hitParticle.Initialize(0);

	reticleObj.Initialize();
	//���f���̓I�u�W�F�N�g�Ɠ�������
	reticleObj.SetModel(Object3d::model);

	reticleSprite.Initialize(reticleTexture);

	reticleSprite.SetPos({ 500.0f,500.0f });

	reticleSprite.SetAnchorPoint({ 0.5f,0.5f });

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

	//�������p�[�e�B�N���X�V
	hitParticle.Update();

	//���e�B�N���̃I�u�W�F�N�g�f�[�^�X�V
	ReticleUpdate();

}

void Player::Draw()
{
	//�����̕`��
	Object3d::Draw();

	//�e�̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

	reticleObj.Draw();
}

void Player::DrawParticle()
{
	hitParticle.Draw();
}

void Player::DrawUI()
{
	reticleSprite.Draw();
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
		const float bulletSpdBase = 8.0f;
		Vector3 velocity(0, 0, bulletSpdBase);

		velocity = reticleObj.GetWorldPosition() - Object3d::GetWorldPosition();
		velocity.normalize();
		velocity *= bulletSpdBase;

		//�e�̐����Ə�����
		std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel, GetWorldPosition(), velocity);

		//�e�̓o�^
		bullets.push_back(std::move(newBullet));
	}


}

void Player::ReticleUpdate()
{
	reticleObj.SetModel(bulletModel);


//���@���烌�e�B�N���ւ̋���(�X�J���[)
	const float distance = 50.0f;
	//���@���烌�e�B�N���ւ̃I�t�Z�b�g
	Vector3 offset = { 0,0,1.0f };
	//���@�̉�]�𔽉f
	offset = Matrix4::transform(offset, matWorld);
	//�x�N�g���̒����𐮂���
	offset.normalize();
	offset *= distance;
	//���W�ݒ�
	reticleObj.position = GetWorldPosition() + offset;
	reticleObj.Update();

	//3d�̃��e�B�N�����W����2D�̃��e�B�N�����W���v�Z
	Vector3 reticlePos = reticleObj.GetWorldPosition();

	//�r���[�|�[�g�s��
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = ;
	matViewPort.m[1][1] = ;
	matViewPort.m[3][0] = ;
	matViewPort.m[3][1] = ;

	//�J�����s��Ƃ̍���

	//�X�N���[�����W�ϊ�

	//���W�ݒ�

}
