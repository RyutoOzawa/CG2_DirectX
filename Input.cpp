#include "Input.h"
#include<wrl.h>


#include<cassert>
using namespace Microsoft::WRL;

void Input::Initialize(WindowsAPI* windowsApi)
{
	HRESULT result_;
	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	this->windowsApi = windowsApi;

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
}

bool Input::IsTrigger(BYTE key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsPress(BYTE key_)
{
	return key[key_];
}

bool Input::IsRelease(BYTE key_)
{
	return (!key[key_] && oldkey[key_]);
}

//�p�b�h�p�֐�

DWORD Input::Updatekeypad(DWORD dwUserIndex)
{
	oldGamePad = gamePad;

	return XInputGetState(
		dwUserIndex,//�����Ȃ���Ă�Ƃ��̑I��
		&gamePad);//���̕ϐ��ɓ��͏󋵂��i�[�����
}

float Input::PadAnalogStickLX()
{
	return (float)gamePad.Gamepad.sThumbLX / SHRT_MAX;
}

float Input::PadAnalogStickLY()
{
	return (float)gamePad.Gamepad.sThumbLY / SHRT_MAX;
}

float Input::PadAnalogStickRX()
{
	return (float)gamePad.Gamepad.sThumbRX / SHRT_MAX;
}

float Input::PadAnalogStickRY()
{
	return (float)gamePad.Gamepad.sThumbRY / SHRT_MAX;
}

float Input::PadLTrigger()
{
	return (float)gamePad.Gamepad.bLeftTrigger / 255;
}

float Input::PadRTrigger()
{
	return (float)gamePad.Gamepad.bRightTrigger / 255;
}

bool Input::PadKey(int button)
{
	return gamePad.Gamepad.wButtons & button;
}

float Input::OldPadAnalogStickLX()
{
	return (float)oldGamePad.Gamepad.sThumbLX / SHRT_MAX;
}

float Input::OldPadAnalogStickLY()
{
	return (float)oldGamePad.Gamepad.sThumbLY / SHRT_MAX;
}

float Input::OldPadAnalogStickRX()
{
	return (float)oldGamePad.Gamepad.sThumbRX / SHRT_MAX;
}

float Input::OldPadAnalogStickRY()
{
	return (float)oldGamePad.Gamepad.sThumbRY / SHRT_MAX;
}

float Input::OldPadLTrigger()
{
	return (float)oldGamePad.Gamepad.bLeftTrigger / 255;
}

float Input::OldPadRTrigger()
{
	return (float)oldGamePad.Gamepad.bRightTrigger / 255;
}

bool Input::TriggerPadKey(int button)
{

	return ((gamePad.Gamepad.wButtons & button) == button && (oldGamePad.Gamepad.wButtons & button) != button);
}

