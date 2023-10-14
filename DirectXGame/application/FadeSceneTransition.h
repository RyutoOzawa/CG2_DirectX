#pragma once
//�t�F�[�h�C���A�E�g
//�V�[���J��(�t�F�[�h�C���A�A�E�g)���s��

#include"BaseSceneTransition.h"
#include"Sprite.h"
#include"EasingData.h"

class FadeSceneTransition:public BaseSceneTransition
{
public:

	//������
	void Initialize() override;

	//�I��
	void Finalize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�V�[������
	void Close()override;

	//�V�[���J����
	void Open()override;

private:

	std::unique_ptr<Sprite> blackSprite = nullptr;
	TextureData* whiteTex = nullptr;

	EasingData  easeSpriteAlpha;

};

