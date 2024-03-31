#pragma once

//�v���C���[�N���X
//���[�U�[�����삷��v���C���[�S�ʂ̏����͂����ōs��

#include"Object3d.h"
#include"Model.h"
#include"Vector3.h"
#include"PlayerBullet.h"
#include<list>
#include"ParticleManager.h"
#include"Sprite.h"
#include"EasingData.h"

class Enemy;

enum class PlayerPhase {
	Spawn,
	Alive,
	Death,
	Leave,
};

//3D�I�u�W�F�N�g�Ȃ̂�obj3d�N���X���p��
class Player :public Object3d
{
public:	//�����o�֐�

	//������
	void Initialize(Model* model_,TextureData* reticleTexture,TextureData* healthTexture);

	//���@��������
	void Spawn();

	//�X�V
	void Update(std::list<std::unique_ptr<Enemy>>* enemys);

	//3D���f���`��
	void Draw();

	//�p�[�e�B�N���`��
	void DrawParticle();

	//UI(2D)�`��
	void DrawUI();

	//�����蔻��R�[���o�b�N
	void OnCollision([[maybe_unused]] const CollisionInfo& info) override;

	//��������
	bool IsAlive()const { return isAlive; }
	//getter
	INT32 GetSpawnTimer() const { return spawnTimer; }
	float GetDistanceCam()const { return distanceCamera; }

	//���@�E�o����
	void Leave();

private://�����o�ϐ�

	//�J��������̎��@�̋���
	const float distanceCamera = 50.0f;

	//���[�J�����W
	Vector3 localPos;

	//��ʏ�̃��e�B�N�����W
	Vector2 reticlePosScreen{WindowsAPI::winW/2.0f,WindowsAPI::winH/2.0f};
	float reticleRadius = 32.0f;

	//�e�֘A
	INT32 shotInterval = 0;
	const INT32 shotCooltime = 15;
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	std::unique_ptr<Model> bulletModel = nullptr;

	//�q�b�g�p�[�e�B�N��
	std::unique_ptr<ParticleManager> hitParticle = nullptr;

	//���e�B�N���֘A
	std::unique_ptr<Object3d> reticleObj = nullptr;
	std::unique_ptr<Sprite> reticleSprite = nullptr;
	Vector4 reticleColor = { 1,1,1,1 };

	//�������o�p�̕�
	static const INT32 haloMax = 8;
	std::unique_ptr<Model> haloModel = nullptr;
	//TODO:���֊֌W��1�̍\���̂ɂ܂Ƃ߂�
	std::array<std::unique_ptr<Object3d>,haloMax> haloObjects;
	std::array<float, haloMax> haloAlphaVel;
	std::array<float, haloMax> haloScaleVel;
	std::array<Vector3, haloMax> haloRotaVel;
	INT32 spawnTimer = 0;
	INT32 spawnTimerMax = 120;
	Easing::EasingData eDataPlayerScale;

	//�E�o���o�֌W	
	static const uint16_t leaveHaloMax = 5;
	uint16_t leaveTimer = 0;
	const uint16_t leaveTimerMax = 120;
	float haloScale[leaveHaloMax] = { 0,0,0,0,0 };

	//HP�֘A
	std::unique_ptr<Sprite> healthSprite = nullptr;
	const int healthMax = 2;
	int health = healthMax;
	int healthWidthMax = 0;
	int healthWidth = healthWidthMax;
	const int damageCooltime = 120;
	int damageInterval = damageCooltime;
	const int deathCountMax = 180;
	int deathCount = deathCountMax;
	bool isAlive = false;
	bool isSpawn = false;
	//���@�̃t�F�[�Y
	PlayerPhase phase = PlayerPhase::Spawn;



	Easing::EasingData easeUIAlpha;
	float UIAlpha = 0.0f;
	std::unique_ptr<Sprite> damageSprite = nullptr;


private://���������p�����o�֐�

	//�㉺���E�ړ�
	void Move();

	//�U��
	void Attack();

	//���e�B�N���̍X�V
	void ReticleUpdate(std::list<std::unique_ptr<Enemy>>* enemys);

	//HP�o�[�̍X�V
	void HealthUpdate();

	//�_���[�W���󂯂����̏���
	void Damage();

	//���񂾏u�Ԃ̏���
	void Death();

	//���S�X�V����
	void UpdateDeath();

	//�����X�V����
	void UpdateSpawn();

	//�E�o�X�V����
	void UpdateLeave();

};

