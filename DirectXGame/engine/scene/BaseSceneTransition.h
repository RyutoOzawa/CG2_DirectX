#pragma once

//��ՃV�[���J�ڃN���X
//�V�[���J�ڂɕK�v�ȉ�ʂ��J�����蓙�̏������s��
//�V�[���J�ڂ�F�X�ł���悤�Ɍp���O��̐݌v�ō��

class GameSceneManager;

enum class TransitionPhase {
	Close,	//����
	Change,	//�ύX�r��
	Open,	//�J��
};


class BaseSceneTransition
{
public:

	//������
	virtual void Initialize() = 0;


	//�I��
	virtual void Finalize() = 0;

	//�X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

	//�V�[������
	virtual void Close() = 0;

	//�V�[���J����
	virtual void Open() = 0;

	//���z�f�X�g���N�^
	virtual ~BaseSceneTransition() = default;

	//�J�ڃt���O
	bool IsTransition()const { return isTransition; }

	//getter
	TransitionPhase GetPhase()const { return transitionPhase; }

protected:
	
	//�V�[���J�ڃt�F�[�Y
	TransitionPhase transitionPhase = TransitionPhase::Close;

	//�V�[���J�ڊǗ��t���O
	bool isTransition = false;
};
