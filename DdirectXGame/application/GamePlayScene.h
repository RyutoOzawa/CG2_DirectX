#pragma once
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


public://�����o�ϐ�

	Input* input = nullptr;
	CollisionManager* collisionManager = nullptr;

	uint32_t backGroundTexture = 0;
	uint32_t marioGraph = 0;
	uint32_t reimuGraph = 0;
	uint32_t particleGraph = 0;
	uint32_t reticleGraph = 0;


	std::unique_ptr<Sprite> backGroundSprite = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Model> defaultModel = nullptr;
	std::unique_ptr<Model> triangleModel = nullptr;
	std::unique_ptr<Model> playerBulletModel = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;
	std::unique_ptr<Object3d> planeObj = nullptr;
	std::unique_ptr<Object3d> triangleObj = nullptr;
	std::unique_ptr<Object3d> rayObj = nullptr;
	std::unique_ptr<FbxModel> model1 = nullptr;
	std::unique_ptr<FbxObject3d> object1 = nullptr;
	std::unique_ptr<ParticleManager> particleMan = nullptr;
	std::unique_ptr<Player> player = nullptr;
	std::list< std::unique_ptr<Enemy>> enemys;
	std::unique_ptr<Object3d> colTestObj = nullptr;


	std::unique_ptr<AudioManager> newAudio = nullptr;
	


	//�����蔻��e�X�g�p
	Vector3 colHitPos;
	Sphere sphere;
	Plane plane;
	Triangle triangle;
	Ray ray;

	Camera* currentCamera = nullptr;
	RailCamera* railCamera = nullptr;

};

