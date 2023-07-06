#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>

//3D�I�u�W�F�N�g�Ȃ̂�obj3d�N���X���p��
class Player :public Object3d
{
public:	//�����o�֐�

	//������
	void Initialize();

	//�X�V
	void Update();

	void Draw();

	void SetBulletModel(Model* model) { bulletModel = model; }

	Vector3 GetPosition()const { return { matWorld.m[3][0],matWorld.m[3][1] ,matWorld.m[3][2] }; }

	void OnCollision(const CollisionInfo& info) override;

private://�����o�ϐ�

	Vector3 pos;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	//�e�̃��f���f�[�^
	Model* bulletModel = nullptr;


private://���������p�����o�֐�

	//�㉺���E�ړ�
	void Move();

	//�U��
	void Attack();

};

