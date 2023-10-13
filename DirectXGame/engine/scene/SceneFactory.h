#pragma once
//�V�[���H��N���X
//�V�[���}�l�[�W������̖��߂ŃV�[�����쐬����N���X
//�V�[���}�l�[�W�����e�V�[���Ɉˑ����Ȃ���

#include"AbstractSceneFactory.h"

//���̃Q�[���p�̃V�[���H��
class SceneFactory : public AbstractSceneFactory
{
public:

	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	/// <returns>���������V�[��</returns>
	GameBaseScene* CreateScene(const std::string& sceneName)override;
};

