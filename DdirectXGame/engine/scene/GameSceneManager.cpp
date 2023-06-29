#include "GameSceneManager.h"
#include<cassert>

GameSceneManager::GameSceneManager()
{

}

GameSceneManager::~GameSceneManager()
{
	
}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager instance;
	return &instance;
}

void GameSceneManager::Update()
{
	
	//���̃V�[�����\�񂳂�Ă���Ȃ�
 	if (nextScene) {
		//���V�[���I��
		if (activeScene) {
			activeScene->Finalize();
			delete activeScene;
		}

		//�V�[���؂�ւ�
		activeScene = nextScene;
		nextScene = nullptr;

		//�V�[��������V�[���؂�ւ����˗��ł���悤�ɁA�V�[���}�l�[�W�����Z�b�g����
		activeScene->SetSceneManager(this);

		//���V�[��������
		activeScene->Initialize();

	}
	//���s�V�[���̍X�V
	activeScene->Update();
}

void GameSceneManager::Draw()
{
	activeScene->Draw();
}

void GameSceneManager::Finalize()
{
	//���݂̃V�[���̏I���ƊJ��
	activeScene->Finalize();
	delete activeScene;
}

void GameSceneManager::ChangeScene(const std::string& sceneName)
{
	//null�`�F�b�N
	assert(sceneFactory);
	assert(nextScene == nullptr);
	//���V�[���̐���
	nextScene = sceneFactory->CreateScene(sceneName);
}
