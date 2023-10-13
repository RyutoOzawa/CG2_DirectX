#pragma once
//�{�X�G�N���X
//�Q�[���Ō�ɓo�ꂷ��{�X�̏������s��

#include"Object3d.h"
#include"Model.h"
#include<array>
#include"SplineCurve.h"
#include"BezierCurve.h"
#include"Sprite.h"
#include"EnemyBullet.h"

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
	void Initialize(Model* bodyModel_,Model* barrelModel_,Object3d* parent_);

	//�X�V
	void Update(const Vector3& playerPos = {0.0f,0.0f,0.0f});

	//�`��
	void Draw();

	void DrawSprite();

	void DrawDebugLine();

	//�I��
	void Finalize();

	//�{�X�̃X�|�[��
	void Spawn(const Matrix4& cameraMatWorld,const Vector3& spawnPos = {0,0,240.0f});

private:
	Model* bodyModel;
	Model* barrelModel;
	std::unique_ptr<Model> bulletModel=nullptr;

	//���֌W
	bool isAlive = false;
	INT32 lifeMax = 10;
	INT32 life = lifeMax;
	TextureData* healthTexture = nullptr;
	std::unique_ptr<Sprite> healthSprite = nullptr;
	float healthSizeWidth = 0.0f;

	//�C��֌W
	static const INT32 barrelMax = 4;
	const float baseBarrelDistance = 20.0f;
	std::array<Object3d, barrelMax> barrelObject;
	std::array<Vector3, barrelMax> barrelDistance;
	std::array<float, barrelMax> barrelRadian;
	Vector3 bulletOutOffset{ 0,0,0 };//���f���̒e���o�镔���̃I�t�Z�b�g
	std::array<EasingData,barrelMax> eDataBarrelMove;	//�o�����̈ړ����C�[�W���O�ɂ���悤
	std::array<Vector3,barrelMax> movePosBeforeBarrel;	//�o�����̈ړ��O���W
	std::array<Vector3,barrelMax> movePosAfterBarrel;	//�o�����̈ړ�����W

	//�C�x���g�V�[������p

	//�{�X�̍s���Ǘ�
	BossAct bossAct = BossAct::Move;
	static const INT32 bossActMax = (INT32)BossAct::BossActMax;
	std::array<INT32, bossActMax> actTime;		//�e�s���Ɏg������
	std::array<INT32, bossActMax> moveCooltime;	//�e�s���̎��s���܂ł̃N�[���^�C��

	INT32 nowActTime = 0;		//���ݍs���̎c�莞��
	INT32 moveInterval = 0;		//���s���Ɉڂ�܂ł̎���

	int count = 0;

	Sprite sp[4];

	//�W�I(���@)�̍��W
	Vector3 targetPos{ 0,0,0 };

	//�ړ��Ȑ�
	float lissajousTheta = 0.0f;
	float lThetaSpd = 0.5f;//���T�[�W���Ȑ��p�p���x
	std::vector<Vector3> curvePoints;
	float radianX = 2.0f;
	float radianY = 3.0f;
	float amplitudeX = 160.0f;
	float amplitudeY = 80.0f;

	//�ˌ��U��
	std::list < std::unique_ptr<EnemyBullet>> bullets;
	static const INT32 shotPosMax = 4;
	std::array<Vector3, shotPosMax> shotPos;//�ˌ����s�����W�z��(0:leftTop 1:leftBottom 2:rightTop 3:rightBottom)
	EasingData eDataMove;//�ړ��p�C�[�W���O�f�[�^
	//�ړ��⊮�p���W2��
	Vector3 movePosBefore{ 0,0,0 };
	Vector3 movePosAfter{ 0,0,0 };
	Vector3 lastPosActMove{ 0,0,0 };//�ړ��t�F�[�Y�̍Ō�̍��W

	//���������֌W
	Vector3 spawnPosOffsetCamera;//�X�|�[������Ƃ��̃J��������̋���(�ڋ�)

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

