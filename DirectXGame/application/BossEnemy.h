#pragma once
#include"Object3d.h"
#include"Model.h"

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

	//�I��
	void Finalize();

private:
	Model* bodyModel;
	Model* BarrelModel;

	BossAct bossAct = BossAct::Move;

	//�e�s���̍X�V����
	void UpdateSpawn();
	void UpdateMove();
	void UpdateAtkShot();
	void UpdateAtkLaser();
	void UpdateDeath();
};

