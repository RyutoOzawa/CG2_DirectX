#include"RailCamera.h"
#include"affine.h"
#include<assert.h>
void RailCamera::Initialize(Vector3 trans, Vector3 rot,const WorldTransform* worldTransform) {

	//���[���h�g�����X�t�H�[���̏����ݒ�
	this->worldTransform_.translation_ = trans;
	this->worldTransform_.rotation_ = rot;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();

	worldTransform_.parent_ = worldTransform;
}
void RailCamera::Update() {

	affine::makeMatIdentity(worldTransform_.matWorld_);
	affine::makeMatRot(worldTransform_.matWorld_, worldTransform_.rotation_);
	affine::makeMatTrans(worldTransform_.matWorld_, worldTransform_.translation_);
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	viewProjection_.eye =Vector3(worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = affine::MatVector(worldTransform_.matWorld_, forward);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	forward += viewProjection_.eye;
	viewProjection_.target = forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = affine::MatVector(worldTransform_.matWorld_, up);
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();

	/*debugText_->SetPos(0.0f, 0.0f);
	debugText_->Printf("eye=%f,%f,%f", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);*/
}