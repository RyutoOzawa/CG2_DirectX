#pragma once
#include<d3d12.h>
#include<vector>

class GpPipeline
{
public:

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};//�p�C�v���C���f�X�N
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = desc.BlendState.RenderTarget[0];//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

public: 
	void Initialize(ID3DBlob* vsBlob,ID3DBlob* psBlob);
	//void SetBlend();


};

