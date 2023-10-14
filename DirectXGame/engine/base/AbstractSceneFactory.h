#pragma once
//�V�[���H��̊��
//�Q�[���ɂ���ăV�[�������ɕK�v�Ȃ��̂��ς�邱�Ƃ��\�z����邽�߁A���N���X�Ƃ��Ă���

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

