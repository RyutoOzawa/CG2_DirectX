#include "Input.h"
#include<wrl.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<cassert>
using namespace Microsoft::WRL;

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result_;
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> directInput = nullptr;
	result_ = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
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
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
