#include "PostEffect.h"

PostEffect::PostEffect() :Sprite(
	//100,			//�e�N�X�`���ԍ�
	//{0,0},			//���W
	//{500.0f,500.0f},//�T�C�Y
	//{1,1,1,1},		//�F
	//{0.0f,0.0f},	//�A���J�[�|�C���g
	//false,			//���E���]�t���O
	//false			//�㉺���]�t���O
)
{

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//��\���Ȃ珈���I��
	//if (isInvisible) {
	//	return;
	//}

	////�e�N�X�`���̐ݒ�R�}���h
	//spriteManager->SetTextureCommand(textureIndex);

	//Update();

	////���_�o�b�t�@�r���[�̐ݒ�
	//spriteManager->directX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	////�萔�o�b�t�@�r���[
	//spriteManager->directX->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	////�`��R�}���h
	//spriteManager->directX->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}
