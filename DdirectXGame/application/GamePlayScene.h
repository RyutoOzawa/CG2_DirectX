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
#include"ParticleManager.h"

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


public://�����o�ϐ�

	Input* input = nullptr;

	uint32_t backGroundTexture = 0;
	uint32_t marioGraph = 0;
	uint32_t reimuGraph = 0;

	std::unique_ptr<Sprite> backGroundSprite = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Model> skydome = nullptr;
	std::unique_ptr<Model> defaultModel = nullptr;
	std::unique_ptr<Model> triangleModel = nullptr;
	std::unique_ptr<Object3d> skydomeObj = nullptr;
	std::unique_ptr<Object3d> planeObj = nullptr;
	std::unique_ptr<Object3d> triangleObj = nullptr;
	std::unique_ptr<Object3d> rayObj = nullptr;

	std::unique_ptr<AudioManager> newAudio = nullptr;

	//�����蔻��e�X�g�p
	Vector3 colHitPos;
	Sphere sphere;
	Plane plane;
	Triangle triangle;
	Ray ray;

	Camera camera;


	//�p�[�e�B�N��
	std::unique_ptr<ParticleManager> particle1;
	std::unique_ptr<ParticleManager> particle2;

	Vector3 particleStart1;
	Vector3 particleStart2;
};

