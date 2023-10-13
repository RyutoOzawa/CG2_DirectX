#pragma once
#include"GameBaseScene.h"
#include"AbstractSceneFactory.h"
#include"BaseSceneTransition.h"

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

	//�I������
	void Finalize();

	/// <summary>
	/// �V�[���ύX�˗�
	/// </summary>
	/// <param name="sceneName">���V�[��</param>
	/// <param name="isTransition">�J�ڂ��s����</param>
	/// <param name="transitionName">�J�ڂ̃p�^�[��</param>
	void ChangeScene(const std::string& sceneName,bool isTransition = true,const std::string& transitionName = "FADE");

	//�V�[���t�@�N�g���[�̃Z�b�^�[
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }

private:
	//���݂̃V�[��
	GameBaseScene* activeScene = nullptr;
	GameBaseScene* nextScene = nullptr;

	//�؂�Ă���V�[���t�@�N�g���[
	AbstractSceneFactory* sceneFactory = nullptr;

	//�V�[���J�ڃN���X
	BaseSceneTransition* sceneTransition = nullptr;

	//���V�[���̗\��̌��m
	void CheckNextScene();

};

