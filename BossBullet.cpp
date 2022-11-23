#include "BossBullet.h"

BossBullet::~BossBullet()
{

}

void BossBullet::Initialize(Object3d* model, const WorldTransform worldTransform, const Vector3& OyaPos, const Vector3& rotaAngle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�����Ŏ󂯎�������̂������o�ϐ��ɑ��
	oyaPos = OyaPos;
	oldPos = worldTransform.translation_;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_.x =worldTransform.matWorld_.m[3][0];
	worldTransform_.translation_.y =worldTransform.matWorld_.m[3][1];
	worldTransform_.translation_.z =worldTransform.matWorld_.m[3][2];

	worldTransform_.rotation_ = rotaAngle;

	

	FloatBlockFlagM = false;
	FloatBlockFlagP = false;
	FloatXRimitFlag = false;

	flyToPlayerFlag = false;
	ToPlayerFlag_ = false;
	attackSound.SoundLoadWave("Resources/Sound/boss1Attack.wav");
}

void BossBullet::Update(const Vector3& playerPos)
{
	// ���W���ړ�������(�P�t���[�����̈ړ��ʂ𑫂�����)
	FlyBlocks(playerPos);
	

	affine::makeAffine(worldTransform_);
	worldTransform_.TransferMatrix();
}

void BossBullet::Draw(const ViewProjection& viewProjection)
{
	if (ToPlayerFlag_ == false) {
		model_->Draw(worldTransform_, viewProjection);
	}
}

Vector3 BossBullet::GetBulletWorldPosition()
{
	return worldTransform_.translation_;
}

void BossBullet::OnCollision()
{
}

void BossBullet::FlyBlocks(Vector3 playerPos)
{
	if (oyaPos.x < worldTransform_.translation_.x) {
		FloatBlockFlagP = true;
	}
	else if (oyaPos.x > worldTransform_.translation_.x) {
		FloatBlockFlagM = true;
	}

	// �v���X�t���O
	if (FloatBlockFlagP == true && FloatXRimitFlag == false) {
		worldTransform_.translation_.x += 0.1f;

		if (worldTransform_.translation_.x >= oldPos.x + 10.0f) {
			// �����Ɉړ�������t���O���I��
			FloatXRimitFlag = true;
			//���������X�|�[��
			flyToPlayerFlag = true;
			attackSound.SoundPlayWave(false, 1.0);
			worldTransform_.translation_.x = oldPos.x + 10.0f;

			// �v���C���[�Ɍ������x�N�g���̌v�Z
			velocity_.x = playerPos.x - worldTransform_.translation_.x;
			velocity_.y = playerPos.y - worldTransform_.translation_.y;
			velocity_.z = playerPos.z - worldTransform_.translation_.z;

			velocity_.normalize();
			velocity_ *= 0.3f;
		}
	}

	// �}�C�i�X�t���O
	if (FloatBlockFlagM == true && FloatXRimitFlag == false) {
		worldTransform_.translation_.x -= 0.1f;

		if (worldTransform_.translation_.x <= oldPos.x - 10.0f) {
			// �����Ɉړ�������t���O���I��
			FloatXRimitFlag = true;
			//���������X�|�[��
			flyToPlayerFlag = true;
			attackSound.SoundPlayWave(false, 1.0);
			worldTransform_.translation_.x = oldPos.x - 10.0f;

			// �v���C���[�Ɍ������x�N�g���̌v�Z
			velocity_.x = playerPos.x - worldTransform_.translation_.x;
			velocity_.y = playerPos.y - worldTransform_.translation_.y;
			velocity_.z = playerPos.z - worldTransform_.translation_.z;

			velocity_.normalize();
			velocity_ *= 0.3f;
		}
	}
	if (ToPlayerFlag_ == false) {
		// �v���C���[�Ɍ������Ĕ�ԏ���
		if (flyToPlayerFlag == true) {
			// ��]���Ȃ�����ł���
			worldTransform_.rotation_.z += 0.5f;

			// �v���C���[�Ɍ������Ĕ��
			worldTransform_.translation_ += velocity_;

			// �f�X�^�C�}�[�̉��Z
			deathTimer_++;

			if (worldTransform_.translation_.y <= 0.0f ||
				worldTransform_.translation_.y >= 30.0f||
				deathTimer_>=kLifeTime) {
				flyToPlayerFlag = false;
				ToPlayerFlag_ = true;
				deathTimer_ = 0;
			}
			
		}
	}
	
	
}

void BossBullet::Reset()
{
	FloatBlockFlagM = false;
	FloatBlockFlagP = false;
	FloatXRimitFlag = false;
	flyToPlayerFlag = false;
	ToPlayerFlag_ = false;
}

void BossBullet::SetToPlayerFlag(bool ToPlayerFlag)
{
	ToPlayerFlag_ = ToPlayerFlag;
}


