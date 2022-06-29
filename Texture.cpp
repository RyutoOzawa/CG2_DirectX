#include "Texture.h"
#include<string.h>
#include<cassert>


void Texture::LoadTexture(const wchar_t texture[])
{
	HRESULT result;
	result = LoadFromWICFile(
		texture,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (!isLoadTexture)isLoadTexture = true;
}

void Texture::Initialize(ReDirectX& directX)
{
	if (isLoadTexture) {
		HRESULT result;
		//�~�b�v�}�b�v����
		result = GenerateMipMaps(
			scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
			TEX_FILTER_DEFAULT, 0, mipChain);
		if (SUCCEEDED(result)) {
			scratchImg = std::move(mipChain);
			metadata = scratchImg.GetMetadata();
		}

		// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
		metadata.format = MakeSRGB(metadata.format);

		//�q�[�v�ݒ�
		textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		textureHeapProp.CPUPageProperty =
			D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		//���\�[�X�ݒ�
		textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureResourceDesc.Format = metadata.format;
		textureResourceDesc.Width = metadata.width;	// ��
		textureResourceDesc.Height = (UINT)metadata.height;	// ����
		textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
		textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
		textureResourceDesc.SampleDesc.Count = 1;

		result = directX.device->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texBuff));

		//�S�~�b�v�}�b�v�ɂ���
		for (size_t i = 0; i < metadata.mipLevels; i++) {
			//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
			const Image* img = scratchImg.GetImage(i, 0, 0);
			//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			result = texBuff->WriteToSubresource(
				(UINT)i,
				nullptr,				//�S�̈�փR�s�[
				img->pixels,			//���f�[�^�A�h���X
				(UINT)img->rowPitch,	//1���C���T�C�Y
				(UINT)img->slicePitch	//�S�T�C�Y
			);
			assert(SUCCEEDED(result));
		}

	}
}

void Texture::CreateSRV(ReDirectX& directX, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle)
{

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directX.device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}
