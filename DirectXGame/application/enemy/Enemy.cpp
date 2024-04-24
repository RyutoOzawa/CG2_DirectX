//�G�N���X
//�Q�[���ɓo�ꂷ��G�P�̂̏������s��

#include "Enemy.h"
#include"SphereCollider.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include"ImguiManager.h"
using namespace Utility;
using namespace Easing;

ParticleManager Enemy::particleManager{};
const INT32 Enemy::shotCoolTime = 120;
Model* Enemy::model = nullptr;
Model* Enemy::bulletModel = nullptr;


void Enemy::EnemyParticleUpdate()
{
	particleManager.Update();
}

void Enemy::EnemyInitialize(TextureData* texData,Model* enemyModel, Model* bulletModel_)
{
	particleManager.Initialize(texData);
	Enemy::model = enemyModel;
	Enemy::bulletModel = bulletModel_;
}

void Enemy::DrawParticle()
{
	particleManager.Draw();
}

void Enemy::Initialize(std::vector<Vector3>& points)
{
	//object3d�̏�����
	Object3d::Initialize();
	SetModel(model);

	moveLine.SetPositions(points);

	

	SetCollider(new SphereCollider({ 0,0,0 }, enemyColSize));
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	scale = baseScale;
	shotInterval = shotCoolTime;

}

void Enemy::Initialize(const Vector3& spawnPos, uint16_t leaveTime_)
{
	//Obj3D�̏�����
	Object3d::Initialize();
	SetModel(model);

	//���܂���W�ɉ����Ĉړ��O�̍��W��ݒ�
	spawnPosBefore = spawnPos;
	if (spawnPos.x > 0.0f) {
		spawnPosBefore.x = 300.0f;
	}
	else {
		spawnPosBefore.x = -300.0f;
	}

	if (spawnPos.y > 0.0f) {
		spawnPosBefore.y = 150.0f;
	}
	else {
		spawnPosBefore.y = -150.0f;
	}

	easeMove.Start(60.0f);

	stayPosition = spawnPos;
	leaveTime = leaveTime_;

	SetCollider(new SphereCollider({ 0,0,0 }, enemyColSize));
	collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
	scale = baseScale;
	shotInterval = shotCoolTime;
}

void Enemy::Update(const Vector3& playerWorldPos, const Matrix4& cameraMat)
{

	//�Ȑ��ɃJ�����s����|���ăJ������ɂ���
	Vector3 cameraPos = { cameraMat.m[3][0],cameraMat.m[3][1] ,cameraMat.m[3][2] };

	//�ړ�
	Move(cameraMat, cameraPos);

	//�U��
	Attack(playerWorldPos);

	//�e�̍X�V
	//����ł�e������
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});


	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}

	//�X�P�[������
	ScaleControll();

	//�C�[�W���O���Ȃ��]
	easeAtkRot.Update();
	if (easeAtkRot.GetTimeRate() < 1.0f) {
		rotation.z =  Lerp(0.0f, 2 * PI, Out( easeAtkRot.GetTimeRate()));
	}
	else {
		rotation.z = 0.0f;
	}

	Object3d::Update();

	//�J�����̃r���[�s��Ɗ|���Z����z���}�C�i�X�Ȃ�E��
	Matrix4 matEnemyView = matWorld * camera->GetView();
	if (matEnemyView.m[3][2] < 25.0f) {
		isAlive = false;
	}

}

void Enemy::Draw()
{
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Draw();
	}

	if (!isAlive) {
		return;
	}

	Object3d::Draw();

}

void Enemy::Spawn()
{
	if (moveLine.GetCPosCount() > 0) {
		moveLine.Start(600, true);
	}

	isAlive = true;
}

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{


	isAlive = false;

	//�p�[�e�B�N���ǉ�
	for (int i = 0; i < 20; i++) {
		Vector3 vel;
		const float baseVel = 2.0f;
		vel.x = Random(-baseVel, baseVel);
		vel.y = Random(-baseVel, baseVel);
		vel.z = Random(-baseVel, baseVel);
		Vector3 acc;
		const float baseAcc = 0.25f;
		acc.x = Random(-baseAcc, baseAcc);
		acc.y = Random(-baseAcc, baseAcc);
		acc.z = Random(-baseAcc, baseAcc);

		particleManager.Add(30, GetWorldPosition(), vel, acc, 10.0f, 0.0f);
	}

}

void Enemy::Attack(const Vector3& playerWorldPos)
{
	//�C���^�[�o����0�Ȃ甭��
	if (shotInterval == 0) {
		//�C���^�[�o�����Z�b�g
		shotInterval = shotCoolTime;
		//�G���玩�@�̃x�N�g���擾
		Vector3 vecEtoP = playerWorldPos - GetLocalPosition();
		vecEtoP.normalize();
		//�e�̑��x
		const float bulletSpdBase = 2.0f;
		vecEtoP *= bulletSpdBase;

		//�e�̐����Ə�����
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(bulletModel, GetWorldPosition(), vecEtoP);

		//�o�^
		bullets.push_back(std::move(newBullet));

		//�ł�������
		float atkScaleSize = 2.5f;

		scale *= atkScaleSize;

		//��]
		easeAtkRot.Start(30.0f);

	}
	else {
		shotInterval--;
		if (shotInterval < 110) {
			collider->SetAttribute(COLLISION_ATTR_ENEMYS);
		}
	}
}

void Enemy::Death()
{
	isAlive = false;
}

void Enemy::Move(const Matrix4& camMat, const Vector3& camPos)
{
	//�ړ��p�Ȑ����ݒ肳��Ă���΋Ȑ��̍X�V
	if (moveLine.GetCPosCount() > 0) {
		moveLine.Update();

		position = Matrix4::transform(moveLine.GetPosition(), camMat) + camPos;
	}
	else{//�ݒ肳��Ă��Ȃ���ΐݒ肳�ꂽ�ޔ����Ԃ����炷
		easeMove.Update();

		Vector3 currentPos = Vector3::Lerp(spawnPosBefore, stayPosition, easeMove.GetTimeRate());

		//currentPos = stayPosition;

		ImGui::Text("current %f,%f,%f", currentPos.x, currentPos.y, currentPos.z);

		position = Matrix4::transform(currentPos,camMat) + camPos;

		if (leaveTime > 0 && easeMove.GetTimeRate() >= 1.0f) {
			leaveTime--;
		}
		else if (leaveTime == 0) {	//�ޔ����Ԃ�0�ɂȂ�����ޔ��s���Ɉڍs
			Leave();
		}
	}

}

void Enemy::Leave()
{
	//�ޔ�p�x�N�g���̎Z�o
	//�r���[�|�[�g�s��
	Matrix4 viewPort;
	viewPort.identity();
	viewPort.m[0][0] = WindowsAPI::winW / 2.0f;
	viewPort.m[1][1] = -(WindowsAPI::winH / 2.0f);
	viewPort.m[3][0] = WindowsAPI::winW / 2.0f;
	viewPort.m[3][1] = WindowsAPI::winH / 2.0f;

	//�J�����s��ƍ��킹��
	Matrix4 matViewProViewPort = Object3d::camera->GetViewProjection() * viewPort;
	//�X�N���[�����W�ϊ�
	Vector3 posScreen = Matrix4::transformDivW(GetWorldPosition(), matViewProViewPort);
	//��ʏ�̂ǂ��ɂ��邩�őޔ��x�N�g�����v�Z
	if (posScreen.x >= WindowsAPI::winW / 2.0f) {
		leaveSpd.x = 1.0f;
	}
	else {
		leaveSpd.x = -1.0f;
	}

	if (posScreen.y > WindowsAPI::winH / 2.0f) {
		leaveSpd.y = -1.0f;
	}
	else {
		leaveSpd.y = 1.0f;
	}
	//Z�����ɂ͈ړ����Ȃ�
	leaveSpd.z = 0.0f;

	//���K�����Ċ�{���x�ɒ���
	leaveSpd.normalize();
	leaveSpd *= leaveSpdBase;

	//���W�ɉ��Z
	stayPosition += leaveSpd;

}

void Enemy::ScaleControll()
{
	//�傫����1�𒴂��Ă����班����������
	//x,y,z�S�ăX�P�[�������l�̂��̂��Ɖ��肵�ăT�C�Y�̒������s��
	if (scale.x > baseScale.x) {
		scale.x -= 0.5f;
		scale.y -= 0.5f;
		scale.z -= 0.5f;
	}
	else if (scale.x < baseScale.x) {
		scale = baseScale;
	}

}

void Enemy::Spin()
{
}
