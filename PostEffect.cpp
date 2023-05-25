#include "PostEffect.h"

PostEffect::PostEffect() :Sprite(
	100,			//�e�N�X�`���ԍ�
	{0,0},			//���W
	{500.0f,500.0f},//�T�C�Y
	{1,1,1,1},		//�F
	{0.0f,0.0f},	//�A���J�[�|�C���g
	false,			//���E���]�t���O
	false			//�㉺���]�t���O
)
{

}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//��\���Ȃ珈���I��
	if (isInvisible) {
		return;
	}

	size = { 500,500 };

	Update();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���̐ݒ�R�}���h
	SetTextureCommand(textureIndex);

	//���_�o�b�t�@�r���[�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	//�`��R�}���h
	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
}
