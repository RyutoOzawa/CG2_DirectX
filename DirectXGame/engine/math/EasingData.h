#pragma once
//�C�[�W���O�f�[�^�N���X1
//�C�[�W���O�Ŏg�����Ԃ̊Ǘ���J�n�����s��

namespace Easing {

	enum class InterType {
		Lerp,		//���`���
		EaseIn,		//�C�[�W���O(�x����)
		EaseOut,	//�C�[�W���O(�����x)
	};

	class EasingData
	{
	private:

		float maxTime = 0;
		float nowTime = 0;
		float timeRate = 0;
		bool isActive = false;

	public:

		//�X�V
		void Update();

		//�J�n
		void Start(float allFrame);

		//getter
		float GetTimeRate() { return timeRate; }

	};

	//�C�[�W���O
	float Out(float t);
	float In(float t);

	float InOut(float t);
	float OutBounce(float t);
	float InBounce(float t);
	float Inback(float t);
}