#pragma once
#include"Object3d.h"
#include"Model.h"
#include<array>
#include"SplineCurve.h"
#include"BezierCurve.h"

//�{�X�̍s���񋓃N���X
enum class BossAct {
	Spawn,			//�X�|�[��
	Move,			//�s��
	AttackShot,		//�U��1
	AttackLaser,	//�U��2
	Death,			//���S
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

	void DrawDebugLine();

	//�I��
	void Finalize();

private:
	Model* bodyModel;
	Model* BarrelModel;

	static const INT32 barrelCount = 4;
	const float baseBarrelDistance = 15.0f;

	std::array<Object3d,barrelCount> barrelObject;
	std::array<Vector3,barrelCount> barrelDistance;
	std::array<float,barrelCount> barrelRadian;

	BossAct bossAct = BossAct::Move;

	SplineCurve moveSpline;
	BezierCurve moveBezier;

	//�Ȑ�
	float lissajousTheta = 0.0f;
	std::vector<Vector3> curvePoints;

	float radianX = 0.0f;
	float radianY = 0.0f;

	float amplitudeX = 0.0f;
	float amplitudeY = 0.0f;

	//�e�s���̍X�V����
	void UpdateSpawn();
	void UpdateMove();
	void UpdateAtkShot();
	void UpdateAtkLaser();
	void UpdateDeath();
};

