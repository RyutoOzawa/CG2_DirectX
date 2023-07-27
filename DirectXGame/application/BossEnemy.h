#pragma once
#include"Object3d.h"
#include"Model.h"
#include<array>
#include"SplineCurve.h"
#include"BezierCurve.h"
#include"Sprite.h"

//�{�X�̍s���񋓃N���X
enum class BossAct {
	Spawn,			//�X�|�[��
	Move,			//�s��
	AttackShot,		//�U��1
	AttackLaser,	//�U��2
	Death,			//���S
	BossActMax,
};

class BossEnemy : public Object3d
{
public: 

	//������
	void Initialize(Model* bodyModel,Model* barrelModel);

	//�X�V
	void Update(const Vector3& playerPos);

	//�`��
	void Draw();

	void DrawSprite();

	void DrawDebugLine();

	//�I��
	void Finalize();

private:
	Model* bodyModel;
	Model* BarrelModel;

	//�C��֌W
	static const INT32 barrelMax = 4;
	const float baseBarrelDistance = 15.0f;
	std::array<Object3d, barrelMax> barrelObject;
	std::array<Vector3, barrelMax> barrelDistance;
	std::array<float, barrelMax> barrelRadian;

	//�{�X�̍s���Ǘ�
	BossAct bossAct = BossAct::Move;
	static const INT32 bossActMax = (INT32)BossAct::BossActMax;
	std::array<INT32, bossActMax> actTime;		//�e�s���Ɏg������
	std::array<INT32, bossActMax> moveCooltime;	//�e�s���̎��s���܂ł̃N�[���^�C��

	INT32 nowActTime = 0;		//���ݍs���̎c�莞��
	INT32 moveInterval = 0;		//���s���Ɉڂ�܂ł̎���

	Sprite sp[4];

	//�ړ��Ȑ�
	float lissajousTheta = 0.0f;
	float lThetaSpd = 0.5f;//���T�[�W���Ȑ��p�p���x
	std::vector<Vector3> curvePoints;
	float radianX = 2.0f;
	float radianY = 3.0f;
	float amplitudeX = 160.0f;
	float amplitudeY = 80.0f;

	//�ˌ��U��
	static const INT32 shotPosMax = 4;
	std::array<Vector3, shotPosMax> shotPos;//�ˌ����s�����W�z��(0:leftTop 1:leftBottom 2:rightTop 3:rightBottom)
	EasingData eDataMove;//�ړ��p�C�[�W���O�f�[�^
	//�ړ��⊮�p���W2��
	Vector3 movePosBefore{ 0,0,0 };
	Vector3 movePosAfter{ 0,0,0 };
	Vector3 lastPosActMove{ 0,0,0 };//�ړ��t�F�[�Y�̍Ō�̍��W

	//�e�s���̍X�V����
	void UpdateSpawn();
	void UpdateMove();
	void UpdateAtkShot();
	void UpdateAtkLaser();
	void UpdateDeath();

	void InitSpawn();
	void InitMove();
	void InitAtkShot();
	void InitAtkLaser();
	void InitDeath();

	void ChangeAct(BossAct nextAct);

	
};

