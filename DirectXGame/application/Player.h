#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include"ParticleManager.h"
#include"Sprite.h"

//3D�I�u�W�F�N�g�Ȃ̂�obj3d�N���X���p��
class Player :public Object3d
{
public:	//�����o�֐�

	//������
	void Initialize(Model* model,uint32_t reticleTexture);

	//�X�V
	void Update();

	void Draw();

	void DrawParticle();

	void DrawUI();

	void SetBulletModel(Model* model) { bulletModel = model; }



	void OnCollision(const CollisionInfo& info) override;

private://�����o�ϐ�

	//�J��������̎��@�̋���
	const float distanceCamera = 50.0f;

	//���[�J�����W
	Vector3 pos;

	//��ʏ�̃��e�B�N�����W
	Vector2 reticlePosScreen{WindowsAPI::winW/2.0f,WindowsAPI::winH/2.0f};

	INT32 shotInterval = 0;
	const INT32 shotCooltime = 5;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	//�e�̃��f���f�[�^
	Model* bulletModel = nullptr;

	//�q�b�g�p�[�e�B�N��
	ParticleManager hitParticle;

	Object3d reticleObj;

	Sprite reticleSprite;


private://���������p�����o�֐�

	//�㉺���E�ړ�
	void Move();

	//�U��
	void Attack();

	//���e�B�N���̍X�V
	void ReticleUpdate();


};

