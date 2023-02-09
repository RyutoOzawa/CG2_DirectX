#include "KEngineFramework.h"

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

	//�X�v���C�g���ʕ��̏�����
	spriteManager = SpriteManager::GetInstance();
	spriteManager->Initialize(directX, WindowsAPI::winW, WindowsAPI::winH);

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
}

void KEngineFramework::Finalize()
{
	imguiManager->Finalize();

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