#pragma once
#include"GameBaseScene.h"
#include<string>

/// <summary>
/// �V�[���H��(�T�O)
/// </summary>
class AbstractSceneFactory
{
public:
	//���z�f�X�g���N�^
	virtual ~AbstractSceneFactory() = default;
	//�V�[������
	virtual GameBaseScene* CreateScene(const std::string& sceneName) = 0;
};

