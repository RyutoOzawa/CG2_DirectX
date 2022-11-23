#pragma once
#include "Object3d.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affine.h"
#include "Input.h"
#include <cassert>
#include"Sprite.h"

class BossPhase_2
{

public:// �T�u�N���X
	enum class Motion {
		standby,
		beam,
		boomerang,
		rush,
		reflector,
	};
	enum class Action
	{
		AttackInterval,
		AttackSelection,
		AttackInMotion
	};

public:// �����o�֐�

	~BossPhase_2();
	// �������֐�
	void Initialize(SpriteManager* spriteManager);

	// �X�V����
	void Update(Vector3 playerPos);

	/// <summary>
/// ���`�ԁ`���`�Ԃ֑J�ڂ���悤�̍X�V����
/// </summary>
	void TitleUpdate();

	// �`�揈��
	void Draw(ViewProjection viewprojection);

	void DrawUI();

	//�Փ˂����o������Ăяo���ꂽ��R�[���o�b�N�֐�
	void OnCollision();

	bool GetBeamFrag() { return beamOBJSetFlag; }

	bool GetBoomerangflg(bool UpOrDown);

	void SetIsDead(bool isDead);

	WorldTransform GetPos() { return worldTransform_[0]; }

	Vector3 GetUpBoomerangPos() { return upBoomerangWorldTransform->translation_; }

	Vector3 GetDownBoomerangPos() { return downBoomerangWorldTransform->translation_; }

	WorldTransform GetBeamTransform() { return beamWorldTransform_; }

	int GetHP() { return HP; }

	float GetMedamaWTTransformY() { return medamaWT.translation_.y; }

	void Rset();

private:
	void beamUpdate(Vector3 playerPos);

	void boomerangUpdate(Vector3 playerPos);

	void rushUpdate(Vector3 playerPos);

	// �s��̓]���ƍX�V
	void TransferMat();

	float Lerp(const float& startPos, const float& endPos, const float& timeRate);

	double easing_Out(double start, double end, double time, double max_time);

	double easing_In(double start, double end, double time, double max_time);
	

	float DegreeToRad(float num)
	{
		return num / 180 * affine::PI;
	}

	float RadToDegree(float num)
	{
		return num / affine::PI * 180;
	}

	// �r�[���֘A�̃��Z�b�g
	void beamReset();

	// �ҋ@���̃v���C���[�Ɍ�������
	void TurnBodyToPlayer(Vector3 playerPos);
	
	// ���񂾂Ƃ��Ɉ�x�����v���C���[�Ɍ�������
	void TurnDeadToPlayer(Vector3 playerPos);

	// �r�[���ł��Ă�Ԃ̃{�X�̌���
	void TurnBeamToPlayer();
	// �ːi�̗\������̎��̃v���C���[�Ɍ�������
	void TurnRushToPlayer(Vector3 playerPos);

	//�u�[�������֘A�̃Z�b�g
	void  boomerangSet(Vector3 playerPos,bool UpOrDown);

	// �ːi�̃��Z�b�g�֐�
	void rushReset();

	// ���S���̔��U����
	void DeathblowUp();

private:// �����o�ϐ�
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_[19];

	// �L���[�u�p�̃X�P�[���̕ϐ�
	float kyubuScale = 3.0f;
	float kyubuLengh = 2.0f;

	// �e�̉�]�v�f
	Vector3 oyaRota;
	Matrix4 oyaRotation;

	// �C���v�b�g
	Input* input_ = nullptr;

	//���f��
	Object3d* model_ = nullptr;

	Vector3 playerPos_;

	const int maxHP = 10;
	int HP = maxHP;
	Sprite* spriteHP;
	Sprite* spriteHPBar;

	Action isAction= Action::AttackInterval;
	const int maxIntervalFrame=200;
	int intervalFrame=0;
	int randAttack=0;
	int Attack=0;
	int oldAttack=0;

	// �r�[���p�̕ϐ�
	#pragma region �r�[���p�̕ϐ�
	// �r�[���̃��[���h�f�[�^
	WorldTransform beamWorldTransform_;
	// �r�[���̃��f��
	Object3d* beamModel_ = nullptr;

	bool beamFlag = false;
	bool beamSetFlag = false;
	bool beamOBJSetFlag = false;
	int beamTimer = 0;
	int beamtoPTimer = 0;

	// �����̎���
	int maxTimer = 6 * 60;
	// �r�[���J�n�̎���
	int maxStartTimer = 3 * 60;
	// �r�[�������J�n�̎���
	int maxEndTimer = 6 * 60;
	// �|�W�V�����X�V����Ԋu
	int beamUpdatePosIntaval = 0.3 * 60;
	// ��������
	int convergenceTimer = 0;
	int maxConvergenceT = 6 * 60;
	
	Vector3 oldPlayerPos;
	Vector3 oldPlayerPos2;
	Vector3 oldVelocity;
	Vector3 beamToPlayerVel;
	Vector3 beamToPlayerVelUp;
	Vector3 beamLength;
	float beamSpeed = 1.0f;
	#pragma endregion

#pragma region �ړ��p�ϐ�

	float angle = 1.57;

	float radius = 50;

#pragma endregion

#pragma region �u�[�������p�ϐ�

	// �����Ƃ��̃t���O
	bool AnnihilationFlag[27];

	Object3d* torunedoModel_ = nullptr;

	WorldTransform torunedoTrans;

	//��̃u�[�������p���[���h�g�����X�t�H�[��
	WorldTransform upBoomerangWorldTransform[5];

	Vector3 upVector;

	bool isUpActive = false;

	bool isUpAttack = false;

	bool isUpPreparation = false;

	//���̃u�[�������悤���[���h�g�����X�t�H�[��
	WorldTransform downBoomerangWorldTransform[5];

	Vector3 downVector;

	bool isDownActive = false;

	bool isDownAttack = false;

	bool isDownDirectionRight = false;

	bool isDownPreparation = false;

#pragma endregion

#pragma region �ːi�p�ϐ�
	// �ːi�J�n�p�̃t���O
	bool rushFlag = false;
	// ������Ƃ��̃|�W�V����Y
	float downPosY = 10.0f;
	// ���̍������L�^����ϐ�
	float originPosY = 0.0f;

	// �J�n���Z�b�g����t���O
	bool rushStartSetFlag = false;

	// �ːi���̃{�X�ƃv���C���[�̃x�N�g��
	Vector3 rushToPlayer;

	// �ːi����Ƃ��̎ԗւ̑���
	int wheelTimer = 0;
	int wheelEndTime = 4 * 60;
	float wheelSpeedX = 0.0f;
	float wheelStart = 0.0f;
	float wheelEnd = 1.5f;
	float wheelDepartureTime = 0.7f;
	// �ԗւ̃X�s�[�h���}�b�N�X�ɓ��B������A�x�N�g�������쐬���邽�߂̃t���O
	bool RtoPFlag = false;

	// �����߂����肪���邩�ǂ����`�F�b�N����t���O
	bool rushFinsh = false;
	bool rushFinshSet = false;
	int wheelTimer2 = 0;
	int wheelEndTime2 = 4 * 60;
	float wheelStart2 = 0.0f;
	float wheelEnd2 = 0.0f;

#pragma endregion

#pragma region ���j�p

	bool blowUpFlag = false;
	bool blowUpSetFlag = false;
	bool blowmatSetFlag = false;
	float randomAngleX = 0, randomAngleY = 0, randomAngleZ = 0;
	float blowStartAngle = (360 * 6) * affine::Deg2Rad;

	Vector3 blowUpVel[19];
	Vector3 blowUpRotaVel={0.4f,0.4f,0.0f};
	WorldTransform Kari;

	int medamawaitTimer = 0;
	int medamaRotaTimer=0;
	int	medamaRotaEndTime = 5 * 60;
	float startMedamaAngle = 0;
	float endMedamaAngle = DegreeToRad(360.0f * 15.0f);
	bool medamaDownFlag = false;
	float medamaGraviti = 0.1;
#pragma endregion

#pragma region �ڋ�
	WorldTransform medamaWT;
	Object3d* medamaModel_ = nullptr;
#pragma endregion

};

