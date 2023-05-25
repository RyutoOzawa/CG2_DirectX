#pragma once
#include"WindowsAPI.h"
#include"DirectX.h"
#include"Input.h"
#include"Texture.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Camera.h"
#include"ImguiManager.h"
#include"AudioManager.h"
#include"GameSceneManager.h"
#include"SceneFactory.h"

#include<memory>

class KEngineFramework
{
public:
	//�����̑S���s
	void Run();

	virtual ~KEngineFramework() = default;

	//������
	virtual void Initialize();

	//�I��
	virtual void Finalize();

	//���t���[���X�V
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	//�I���`�F�b�N
	virtual bool IsEndRequest() { return endRequest; }

	bool endRequest = false;
	WindowsAPI* windowsAPI = nullptr;
	ReDirectX* directX = nullptr;
	Input* input = nullptr;
	ImguiManager* imguiManager = nullptr;

protected:
	GameSceneManager* sceneManager = nullptr;
	//�V�[���t�@�N�g���[
	std::unique_ptr<AbstractSceneFactory> sceneFactory = nullptr;
};

