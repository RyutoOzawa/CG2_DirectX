#include "GameSceneManager.h"
#include<cassert>
#include"FadeSceneTransition.h"

GameSceneManager::GameSceneManager()
{

}

GameSceneManager::~GameSceneManager()
{
	
}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager* instance = nullptr;
	if (instance == nullptr) {
		instance = new GameSceneManager;
	}
	return instance;
}

void GameSceneManager::Update()
{





	//�V�[���J�ڂ���������Ă���Ȃ�J�ڂ�����
	if (sceneTransition) {
		sceneTransition->Update();

		//�V�[���J�ڃt�F�[�Y��change�Ȃ�V�[���̕ύX
		if (sceneTransition->GetPhase() == TransitionPhase::Change) {
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
			activeScene->Update();
			//�V�[�����J����
			sceneTransition->Open();

		}

		//�V�[���J�ڊǗ��t���O���~��Ă���΃V�[���J�ڂ�����
		if (!sceneTransition->IsTransition()) {
			delete sceneTransition;
			sceneTransition = nullptr;
		}
	}
	else {

		//���V�[���̗\������m
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
}

void GameSceneManager::Draw()
{
	if (activeScene) {
		activeScene->Draw();
	}

	if (sceneTransition) {

		sceneTransition->Draw();
	}
}


void GameSceneManager::Finalize()
{
	//���݂̃V�[���̏I���ƊJ��
	activeScene->Finalize();
	delete activeScene;
}

void GameSceneManager::ChangeScene(const std::string& sceneName,bool isTransition, const std::string& transitionName)
{
	//null�`�F�b�N
	assert(sceneFactory);
	assert(nextScene == nullptr);
	//���V�[���̐���
	nextScene = sceneFactory->CreateScene(sceneName);

	//�J�ڂ��s��Ȃ��Ȃ琶�����Ȃ�
	if (!isTransition) {
		return;
	}

	//�����ɑΉ������J�ڃN���X�𐶐�(�f�t�H���g�̓t�F�[�h)
	if (transitionName == "FADE") {
		sceneTransition = new FadeSceneTransition();

	}

	//�V�[���J�ڏ�����
	sceneTransition->Initialize();
	sceneTransition->Close();
}
