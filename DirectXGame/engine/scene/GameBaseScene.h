#pragma once
//�V�[����ՃN���X
//�V�[���쐬�̑匳�ƂȂ���N���X

class GameSceneManager;

class GameBaseScene
{
public:
	//������
	virtual void Initialize() = 0;

	//�I��
	virtual void Finalize() = 0;

	//�X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

	virtual ~GameBaseScene() = default;

	virtual void SetSceneManager(GameSceneManager* sceneManager_) { this->sceneManager = sceneManager_; }

protected:
	//�؂�Ă���V�[���}�l�[�W��
	GameSceneManager* sceneManager = nullptr;
};

