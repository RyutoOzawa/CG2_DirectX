#pragma once

#include"DirectX.h"
#include"windows.h"
#include"WindowsAPI.h"
#include<Xinput.h>

#define DIRECTINPUT_VERSION 0x0800		//DirectInput�̃o�[�W�����w��
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<wrl.h>
#pragma comment (lib, "xinput.lib")

class Input
{
public:

private://�L�[�{�[�h�p�����o�ϐ�
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256]{};
	BYTE oldkey[256]{};

	WindowsAPI* windowsApi = nullptr;
public://�L�[�{�[�h�p�����o�֐�
	void Initialize(WindowsAPI* windowsApi);
	void Update();
	bool IsTrigger(BYTE key_);
	bool IsPress(BYTE key_);
	bool IsRelease(BYTE key_);
private://�p�b�h�p�����o�ϐ�
	XINPUT_STATE gamePad;
	XINPUT_STATE oldGamePad;

public://�p�b�h�p�����o�֐�
	/// <summary>
	/// �R���g���[���[�̏󋵂̎擾�i���t���[������j
	/// </summary>
	///<param name="dwUserIndex">�Ȃ���Ă�R���g���[���[�������̏ꍇ�̎w��i��Ȃ�0��OK�j</param>
	/// <returns>�������H</returns>
	DWORD Updatekeypad(DWORD dwUserIndex);

	/// <summary>
	/// ���A�i���O�X�e�B�b�N��X�����̓��͏�
	/// </summary>
	/// <returns>���A�i���O�X�e�B�b�N��X�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickLX();

	/// <summary>
	/// ���A�i���O�X�e�B�b�N��Y�����̓��͏�
	/// </summary>
	/// <returns>���A�i���O�X�e�B�b�N��Y�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickLY();

	/// <summary>
	/// �E�A�i���O�X�e�B�b�N��X�����̓��͏�
	/// </summary>
	/// <returns>�E�A�i���O�X�e�B�b�N��X�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickRX();

	/// <summary>
	/// �E�A�i���O�X�e�B�b�N��Y�����̓��͏�
	/// </summary>
	/// <returns>�E�A�i���O�X�e�B�b�N��Y�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float PadAnalogStickRY();

	/// <summary>
	/// ���g���K�[(LZ�Ƃ��Ă΂����)�̓��͏�
	/// </summary>
	/// <returns>���g���K�[�̓��͏󋵂�Ԃ�(0�`+1)</returns>
	float PadLTrigger();

	/// <summary>
	/// �E�g���K�[(RZ�Ƃ��Ă΂����)�̓��͏�
	/// </summary>
	/// <returns>���g���K�[�̓��͏󋵂�Ԃ�(0�`+1)</returns>
	float PadRTrigger();

	/// <summary>
	/// �R���g���[���[�̃{�^���̓��͏�
	/// </summary>
	/// ///<param name="button">�{�^���̎��(XINPUT_GAMEPAD_�Ŏn�܂�萔�Ŏw��)</param>
	/// <returns>������Ă邩�H</returns>
	bool PadKey(int button);

	/// <summary>
	/// ��t���[���O�̍��A�i���O�X�e�B�b�N��X�����̓��͏�
	/// </summary>
	/// <returns>��t���[���O�̍��A�i���O�X�e�B�b�N��X�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float OldPadAnalogStickLX();

	/// <summary>
	/// ��t���[���O�̍��A�i���O�X�e�B�b�N��Y�����̓��͏�
	/// </summary>
	/// <returns>��t���[���O�̍��A�i���O�X�e�B�b�N��Y�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float OldPadAnalogStickLY();

	/// <summary>
	/// ��t���[���O�̉E�A�i���O�X�e�B�b�N��X�����̓��͏�
	/// </summary>
	/// <returns>��t���[���O�̉E�A�i���O�X�e�B�b�N��X�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float OldPadAnalogStickRX();

	/// <summary>
	/// ��t���[���O�̉E�A�i���O�X�e�B�b�N��Y�����̓��͏�
	/// </summary>
	/// <returns>��t���[���O�̉E�A�i���O�X�e�B�b�N��Y�����̓��͏󋵂�Ԃ�(-1�`+1)</returns>
	float OldPadAnalogStickRY();

	/// <summary>
	/// ��t���[���O�̍��g���K�[(LZ�Ƃ��Ă΂����)�̓��͏�
	/// </summary>
	/// <returns>��t���[���O�̍��g���K�[�̓��͏󋵂�Ԃ�(0�`+1)</returns>
	float OldPadLTrigger();

	/// <summary>
	/// ��t���[���O�̉E�g���K�[(RZ�Ƃ��Ă΂����)�̓��͏�
	/// </summary>
	/// <returns>��t���[���O�̍��g���K�[�̓��͏󋵂�Ԃ�(0�`+1)</returns>
	float OldPadRTrigger();

	/// <summary>
	/// ��t���[���O�̃R���g���[���[�̃{�^���̓��͏�
	/// </summary>
	/// ///<param name="button">�{�^���̎��(XINPUT_GAMEPAD_�Ŏn�܂�萔�Ŏw��)</param>
	/// <returns>��t���[���O�ɉ�����Ă邩�H</returns>
	bool TriggerPadKey(int button);
};

