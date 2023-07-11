#include "KEngineFramework.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"ParticleManager.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
	//windowsAPI����������
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX����������
	directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	//�L�[�{�[�h����������
	input =  Input::GetInstance();
	input->Initialize(windowsAPI);

	//�e�N�X�`���}�l�[�W���[�̏�����
	Texture::Initialize(directX->GetDevice());

	//�X�v���C�g�̏�����
	Sprite::StaticInitialize(directX->GetDevice(), directX->GetCommandList(), WindowsAPI::winW, WindowsAPI::winH);

	//3D�I�u�W�F�N�g�̏�����
	Object3d::StaticInitialize(directX);

	//�J�����N���X������
	Camera::StaticInitialize(directX->GetDevice());

	//�I�[�f�B�I������
	AudioManager::StaticInitialize();

	//�f�o�b�O�e�L�X�g(imgui������)
	imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize(windowsAPI,directX);

	//�V�[���}�l�[�W���̐���
	sceneManager = GameSceneManager::GetInstance();

	//FBX������
	FbxLoader::GetInstance()->Initialize(directX->GetDevice());

	//FbxObject�̃f�o�C�X�ƃp�C�v���C������
	FbxObject3d::SetDevice(directX->GetDevice());
	FbxObject3d::SetCmdList(directX->GetCommandList());
	FbxObject3d::CreateGraphicsPipeline();


	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize(directX->GetDevice());

	//�p�[�e�B�N���}�l�[�W��������
	ParticleManager::StaticInitialize(directX);

}

void KEngineFramework::Finalize()
{


	imguiManager->Finalize();

	sceneManager->Finalize();

	FbxLoader::GetInstance()->Finalize();

	windowsAPI->Finalize();

	delete directX;
	delete windowsAPI;
}

void KEngineFramework::Update()
{
	//windows�̃��b�Z�[�W����
	if (windowsAPI->ProcessMessage()) {
		//���[�v�𔲂���
		endRequest = true;
	}

	//���͌n�X�V
	input->Update();

	//�V�[���}�l�[�W���̍X�V����
	sceneManager->Update();
}

void KEngineFramework::Run()
{
	//������
	Initialize();

	//�Q�[�����[�v
	while (true)
	{
		//���t���[���X�V
		Update();
		if (IsEndRequest()) {
			break;
		}
		//�`��

		Draw();
	}

	//�I������
	Finalize();
}
