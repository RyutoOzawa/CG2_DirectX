#include "Input.h"


void Input::Initialize(HRESULT result_,WindowsAPI windowsAPI_)
{
	result_ = DirectInput8Create(
		windowsAPI_.w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result_));

	result_ = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result_));

	//���̓f�[�^�`���̃Z�b�g
	result_ = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result_));

	//�r�����䃌�x���̃Z�b�g
	result_ = keyboard->SetCooperativeLevel(
		windowsAPI_.hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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

bool Input::IsTrigger(unsigned char key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsPress(unsigned char key_)
{
	return key[key_];
}

bool Input::IsRelease(unsigned char key_)
{
	return (!key[key_] && oldkey[key_]);
}
