#include "Player.h"
#include"Input.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"

void Player::Initialize(Model* model, uint32_t reticleTexture)
{
	Object3d::Initialize();
	SetModel(model);

	position = { 0,0,distanceCamera };

	//�R���C�_�[�̒ǉ�
	float radius = 1.0f;
	//���a���������������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3(0, 0, 0),radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	hitParticle.Initialize(0);

	reticleObj.Initialize();
	//���f���̓I�u�W�F�N�g�Ɠ�������
	reticleObj.SetModel(Object3d::model);

	reticleSprite.Initialize(reticleTexture);

//	reticleSprite.SetPos({ 500.0f,500.0f });

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
	//����
	INT32 inputHorizontal = 0;
	INT32 inputVertical = 0;

	if (Input::GetInstance()->IsKeyPress(DIK_A) || Input::GetInstance()->IsDownLStickLeft()) {
		inputHorizontal = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_D) || Input::GetInstance()->IsDownLStickRight()) {
		inputHorizontal = 1;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_W) || Input::GetInstance()->IsDownLStickUp()) {
		inputVertical = 1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_S) || Input::GetInstance()->IsDownLStickDown()) {
		inputVertical = -1;
	}

	//���W�ɉ��Z���鑬�x
	Vector3 spd{};
	float baseSpd = 0.5f;


	spd.x += baseSpd * inputHorizontal;
	spd.y += baseSpd * inputVertical;
	



	//���ݍ��W���擾
	pos = position;
	//���Z���đ��
	pos += spd;
	position = pos;

	const float bodyTurnBase = (float)PI / 180.0f;

	//����̋���
	rotation.z -= bodyTurnBase * inputHorizontal;
	rotation.x -= bodyTurnBase * inputVertical;

	if (inputHorizontal == 0) {
		if (rotation.z > 0) {
			rotation.z -= bodyTurnBase * 3.0f;
		}
		else if (rotation.z < 0) {
			rotation.z += bodyTurnBase * 3.0f;
		}
		else {
			rotation.z = 0.0f;
		}
	}

	if (inputVertical == 0) {
		if (rotation.x > 0) {
			rotation.x -= bodyTurnBase * 3.0f;
		}
		else if (rotation.x < 0) {
			rotation.x += bodyTurnBase * 3.0f;
		}
		else {
			rotation.x = 0.0f;
		}
	}

	//obj3d�̍X�V
	Object3d::Update();

}

void Player::Attack()
{
	//�X�y�[�X�L�[�Œe����
	if (shotInterval == 0) {

		if (Input::GetInstance()->IsKeyPress(DIK_SPACE) || Input::GetInstance()->IsPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			
			shotInterval = shotCooltime;

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
	else {
		shotInterval--;
	}


}

void Player::ReticleUpdate()
{
	reticleObj.SetModel(bulletModel);

	//�r���[�|�[�g�s��
	Matrix4 matViewPort;
	matViewPort.identity();
	matViewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	matViewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	matViewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	//�J�����s��Ƃ̍���
	Matrix4 matViewProViewPort = Object3d::camera->GetViewProjection() * matViewPort;


	//��ʏ�̃��e�B�N�����W�𓮂���
	Vector2 reticleSpd = { 0,0 };
	float reticleSpdBase = 8.0f;

	//����
	INT32 inputHorizontal = 0;
	INT32 inputVertical = 0;

	if (Input::GetInstance()->IsKeyPress(DIK_LEFT) || Input::GetInstance()->IsDownRStickLeft()) {
		inputHorizontal = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_RIGHT) || Input::GetInstance()->IsDownRStickRight()) {
		inputHorizontal = 1;
	}

	if (Input::GetInstance()->IsKeyPress(DIK_UP) || Input::GetInstance()->IsDownRStickUp()) {
		inputVertical = -1;
	}
	else if (Input::GetInstance()->IsKeyPress(DIK_DOWN) || Input::GetInstance()->IsDownRStickDown()) {
		inputVertical = 1;
	}

	

	reticleSpd.x = inputHorizontal * reticleSpdBase;
	reticleSpd.y = inputVertical * reticleSpdBase;

	reticlePosScreen += reticleSpd;

	//���W���X�v���C�g�ɃZ�b�g
	reticleSprite.SetPos(reticlePosScreen);

	//�r���[�A�ˉe�A�r���[�|�[�g�̍s�������
	Matrix4 matVBVInverse = matViewProViewPort;
	matVBVInverse.Inverse();

	//�X�N���[�����W
	Vector3 posNear = {reticlePosScreen.x, reticlePosScreen.y, 0};
	Vector3 posFar = {reticlePosScreen.x, reticlePosScreen.y, 1};

	//���[���h���W�n�ɕϊ�
	posNear = Matrix4::transformDivW(posNear, matVBVInverse);
	posFar = Matrix4::transformDivW(posFar, matVBVInverse);

	//���C�̕���
	Vector3 direction = posFar - posNear;
	direction.normalize();

	//�J�������烌�e�B�N��(3D)�ւ̋���
	const float distanceReticle3D = distanceCamera + 25.0f;
	reticleObj.position = posNear + direction * distanceReticle3D;


////���@���烌�e�B�N���ւ̋���(�X�J���[)
//	const float distance = 50.0f;
//	//���@���烌�e�B�N���ւ̃I�t�Z�b�g
//	Vector3 offset = { 0,0,1.0f };
//	//���@�̉�]�𔽉f
//	offset = Matrix4::transform(offset, matWorld);
//	//�x�N�g���̒����𐮂���
//	offset.normalize();
//	offset *= distance;
//	//���W�ݒ�
//	reticleObj.position = GetWorldPosition() + offset;
	reticleObj.Update();
//
//	////3d�̃��e�B�N�����W����2D�̃��e�B�N�����W���v�Z
//	Vector3 reticlePos = reticleObj.GetWorldPosition();
//
//	//
//
//	////�X�N���[�����W�ϊ�
//	reticlePos = Matrix4::transformDivW(reticlePos, matViewProViewPort);
//
//	//���W�ݒ�
//	reticleSprite.SetPos( { reticlePos.x, reticlePos.y });

}
