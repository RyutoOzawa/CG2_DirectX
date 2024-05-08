//���̓N���X
//�L�[�{�[�h�A�p�b�h�̐ڑ�����͂��擾����N���X

#include "Input.h"
#include<wrl.h>
#include<cassert>
using namespace Microsoft::WRL;
#include"ImguiManager.h"
#include"Util.h"
using namespace Utility;


Input::Input()
{
}

Input::~Input()
{
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WindowsAPI* winApp)
{
	HRESULT result_;
	//WinApp�̃C���X�^���X���擾
	WindowsAPI* windowsApi = winApp;


	//DirectInput�̃C���X�^���X����
	result_ = DirectInput8Create(
		windowsApi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result_));
	//�L�[�{�[�h�f�o�C�X����
	result_ = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result_));
	//���̓f�[�^�`���̃Z�b�g
	result_ = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result_));
	//�r�����䃌�x���̃Z�b�g
	result_ = keyboard->SetCooperativeLevel(
		windowsApi->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result_));
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�O�t���[���̃L�[����ۑ�
	for (int i = 0; i < 256; i++) {
		oldkey[i] = key[i];
	}
	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);

	//�p�b�h�̐ڑ��m�F
	oldPadState = padState;
	DWORD flag;
	flag = XInputGetState(0, &padState);


}

bool Input::IsKeyTrigger(BYTE key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsKeyPress(BYTE key_)
{
	return key[key_];
}

bool Input::IsKeyRelease(BYTE key_)
{
	return (!key[key_] && oldkey[key_]);
}

bool Input::IsPadTrigger(WORD Button)
{
	return (padState.Gamepad.wButtons & Button) && ((oldPadState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsPadPress(WORD Button)
{
	return padState.Gamepad.wButtons & Button;
}

bool Input::IsPadRelease(WORD Button)
{
	return (oldPadState.Gamepad.wButtons & Button) && ((padState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsDownLStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbLX < -deadZone;
}

bool Input::IsDownLStickRight(int deadZone)
{
	return padState.Gamepad.sThumbLX > deadZone;
}

bool Input::IsDownLStickUp(int deadZone)
{
	return padState.Gamepad.sThumbLY > deadZone;
}

bool Input::IsDownLStickDown(int deadZone)
{
	return padState.Gamepad.sThumbLY < -deadZone;
}

bool Input::IsDownRStickLeft(int deadZone)
{
	ImGui::Text("%d", padState.Gamepad.sThumbRX);
	return padState.Gamepad.sThumbRX < -deadZone;
}

bool Input::IsDownRStickRight(int deadZone)
{
	ImGui::Text("%d", padState.Gamepad.sThumbRX);

	return padState.Gamepad.sThumbRX > deadZone;

}

bool Input::IsDownRStickUp(int deadZone)
{
	return padState.Gamepad.sThumbRY > deadZone;
}

bool Input::IsDownRStickDown(int deadZone)
{
	return padState.Gamepad.sThumbRY < -deadZone;
}

float Input::GetDownRstickX(int deadZone)
{
	//�|�ꂽ���̍ő�l
	int downMax = 32768;
	//�|�ꂽ�傫�����f�b�h�]�[���𒴂����炻������-1~1�ŕ��
	if (fabs(padState.Gamepad.sThumbRX) > deadZone) {

		downMax -= deadZone;
		int stickDown = (int)fabs(padState.Gamepad.sThumbRX) - deadZone;
		float stickRate = (float)stickDown / (float)downMax;
		stickRate *= Clump(padState.Gamepad.sThumbRX, -1.0f, 1.0f);
		return stickRate;
	}

	return 0.0f;
}

float Input::GetDownRstickY(int deadZone)
{
	//�|�ꂽ���̍ő�l
	int downMax = 32768;
	//�|�ꂽ�傫�����f�b�h�]�[���𒴂����炻������-1~1�ŕ��
	if (fabs(padState.Gamepad.sThumbRY) > deadZone) {

		downMax -= deadZone;
		int stickDown = (int)fabs(padState.Gamepad.sThumbRY) - deadZone;
		float stickRate = (float)stickDown / (float)downMax;
		stickRate *= Clump(padState.Gamepad.sThumbRY, -1.0f, 1.0f);
		return stickRate;
	}

	return 0.0f;
}

float Input::GetDownLstickX(int deadZone)
{
	//�|�ꂽ���̍ő�l
	int downMax = 32768;
	//�|�ꂽ�傫�����f�b�h�]�[���𒴂����炻������-1~1�ŕ��
	if (fabs(padState.Gamepad.sThumbLX) > deadZone) {

		downMax -= deadZone;
		int stickDown = (int)fabs(padState.Gamepad.sThumbLX) - deadZone;
		float stickRate = (float)stickDown / (float)downMax;
		stickRate *= Clump(padState.Gamepad.sThumbLX, -1.0f, 1.0f);
		return stickRate;
	}

	return 0.0f;
}

float Input::GetDownLstickY(int deadZone)
{
	//�|�ꂽ���̍ő�l
	int downMax = 32768;
	//�|�ꂽ�傫�����f�b�h�]�[���𒴂����炻������-1~1�ŕ��
	if (fabs(padState.Gamepad.sThumbLY) > deadZone) {

		downMax -= deadZone;
		int stickDown = (int)fabs(padState.Gamepad.sThumbLY) - deadZone;
		float stickRate = (float)stickDown / (float)downMax;
		stickRate *= Clump(padState.Gamepad.sThumbLY, -1.0f, 1.0f);
		return stickRate;
	}

	return 0.0f;
}
