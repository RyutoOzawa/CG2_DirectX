#pragma once
#include"GameBaseScene.h"
#include"AbstractSceneFactory.h"

class GameSceneManager
{
private:
	GameSceneManager();
	~GameSceneManager();

public:

	//�R�s�[�R���X�g���N�^����
	GameSceneManager(const GameSceneManager& obj) = delete;
	//������Z�q�𖳌�
	GameSceneManager& operator=(const GameSceneManager& obj) = delete;

	//�C���X�^���X�A�N�Z�X��p�֐�
	static GameSceneManager* GetInstance();

	//�X�V
	void Update();

	//�`��
	void Draw();

	void ChangeScene(const std::string& sceneName);

	//�V�[���t�@�N�g���[�̃Z�b�^�[
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }

private:
	//���݂̃V�[��
	GameBaseScene* activeScene = nullptr;
	GameBaseScene* nextScene = nullptr;

	//�؂�Ă���V�[���t�@�N�g���[
	AbstractSceneFactory* sceneFactory = nullptr;


};

