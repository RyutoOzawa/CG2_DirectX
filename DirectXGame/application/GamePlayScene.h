#pragma once
//�Q�[���V�[���N���X
//�Q�[���{�҂̏������s��

#include"GameBaseScene.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include"Camera.h"
#include"Object3d.h"
#include"AudioManager.h"
#include<memory>
#include"CollisionPrimitive.h"
#include"FbxModel.h"
#include"FbxObject3d.h"
#include"ParticleManager.h"
#include"Player.h"
#include"RailCamera.h"
#include"Enemy.h"
#include"CollisionManager.h"
#include"BossEnemy.h"

enum class GamePhase {
	Event_GameStart,//�Q�[���J�n�C�x���g(���@�̏o��)
	Game_Main,//���C���̃Q�[��(�G���G��)
	Event_BossSpawn,//�{�X�̃X�|�[���C�x���g
	Game_Boss,//�{�X��
	Event_GameClear,//�Q�[���N���A�C�x���g

};

class GamePlayScene :public GameBaseScene
{
public:
	//������
	void Initialize()override;

	//�I��
	void Finalize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	void EnemySpawn();


private://�����o�ϐ�

	Input* input = nullptr;
	CollisionManager* collisionManager = nullptr;

	TextureData* backGroundTexture = nullptr;
	TextureData* marioGraph = nullptr;
	TextureData* reimuGraph = nullptr;
	TextureData* particleGraph = nullptr;
	TextureData* whiteGraph = nullptr;
	TextureData* reticleGraph = nullptr;
	TextureData* testTex1 = nullptr;
	TextureData* testTex2 = nullptr;
	TextureData* testTex3 = nullptr;


	std::unique_ptr<Sprite> backGroundSprite = nullptr;
	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Model> defaultModel = nullptr;
	std::unique_ptr<Model> playerModel = nullptr;
	std::unique_ptr<Model> playerBulletModel = nullptr;
	std::unique_ptr<Model> enemyModel = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;
	std::unique_ptr<FbxModel> model1 = nullptr;
	std::unique_ptr<FbxObject3d> object1 = nullptr;
	std::unique_ptr<Player> player = nullptr;
	std::list< std::unique_ptr<Enemy>> enemys;
	std::unique_ptr<Object3d> colTestObj = nullptr;

	std::unique_ptr<AudioManager> newAudio = nullptr;

	//�{�X�@
	std::unique_ptr<BossEnemy> boss = nullptr;

	std::unique_ptr<Model> bossBodyModel = nullptr;
	std::unique_ptr<Model> bossBarrelModel = nullptr;


	Camera* currentCamera = nullptr;
	RailCamera* railCamera = nullptr;

	//�Q�[���t�F�[�Y
	GamePhase gamePhase = GamePhase::Event_GameStart;

private:

	//�e�t�F�[�Y�̍X�V
	//�S�t�F�[�Y�Ő�΂ɍX�V���鏈��
	void UpdateAllPhase();	
	//�Q�[���t�F�[�Y�ōX�V���鏈��
	void UpdateGamePhase();	
	//�Q�[���J�n
	void UpdateGameStart();
	//���C���Q�[��
	void UpdateMain();		
	//�{�X����
	void UpdateBossSpawn();	
	//�{�X��
	void UpdateBoss();		
	//�Q�[���N���A
	void UpdateClear();		

};

