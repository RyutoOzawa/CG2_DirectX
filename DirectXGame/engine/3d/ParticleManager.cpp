#include "ParticleManager.h"
using namespace Microsoft::WRL;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"GpPipeline.h"
#include"Util.h"
#include"Texture.h"

ComPtr<ID3D12PipelineState> ParticleManager::pipelineState;
ComPtr<ID3D12RootSignature> ParticleManager::rootSignature;
ReDirectX* ParticleManager::directX = nullptr;
Camera* ParticleManager::camera = nullptr;

void ParticleManager::StaticInitialize(ReDirectX* directX_)
{
	assert(directX_);
	directX = directX_;

	//�p�C�v���C���ݒ�
	CreatePipeline3D();
}

void ParticleManager::BeginDraw(Camera* camera_)
{
	camera = camera_;

	//�p�C�v���C���X�e�[�g�̐ݒ�
	directX->GetCommandList()->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	directX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`��̐ݒ�
	directX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//2�Ԓ萔�o�b�t�@�r���[�ɃJ�����̒萔�o�b�t�@��ݒ�
	directX->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuff->GetGPUVirtualAddress());

}

void ParticleManager::Initialize(TextureData* texData)
{
	//�e�N�X�`���Z�b�g
	SetTexture(texData);

	ID3D12Device* device = directX->GetDevice();

	HRESULT result;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));

	Matrix4 matrix = matrix.identity();
	constMap->matBillboard = matrix;

	

	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosScale) * vertexCount);

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexPosScale* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	//for (int i = 0; i < vertices.size(); i++) {
	//	vertMap[i] = vertices[i]; // ���W���R�s�[
	//}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(VertexPosScale);
}

void ParticleManager::Update()
{
	//�����������p�[�e�B�N����S�폜
	particles.remove_if([](Particle& x) {return x.frame > x.num_frame; });

	//�S�p�[�e�B�N���X�V
	for (std::forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
		//�o�߃t���[�������J�E���g
		it->frame++;
		//���x�ɉ����x�����Z
		it->velocity += it->accel;
		//���x�ɂ��ړ�
		it->position += it->velocity;
		//�p�[�e�B�N���̌��ݎ��Ԃ��v�Z
		float f = (float)it->frame / it->num_frame;
		//�X�P�[���̐��`���
		it->scale = (it->scaleEnd - it->scaleStart) * f;
		it->scale += it->scaleStart;

	}
	//���_�o�b�t�@�փf�[�^�]��
	VertexPosScale* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//�p�[�e�B�N���̏���1�����f
		for (std::forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
			//���W
			vertMap->pos = it->position;
			//�X�P�[��
			vertMap->scale = it->scale;
			//���̏��Ԃ�
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}
}

void ParticleManager::Draw()
{
	matBillboard.identity();

	//�J�����̊e�x�N�g����ݒ�
	Vector3 eye, target, up;
	eye = camera->eye;
	target = camera->target;
	up = camera->up;
	Vector3 cameraAxisZ, cameraAxisX, cameraAxisY;
	cameraAxisZ = target - eye;
	cameraAxisZ.normalize();
	cameraAxisX = up.cross(cameraAxisZ);
	cameraAxisX.normalize();
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	cameraAxisY.normalize();

	Matrix4 matCameraRot = {
		cameraAxisX.x,cameraAxisX.y,cameraAxisX.z,0,
		cameraAxisY.x,cameraAxisY.y,cameraAxisY.z,0,
		cameraAxisZ.x,cameraAxisZ.y,cameraAxisZ.z,0,
					0,            0,            0,1,
	};

	//�J�����̍s����r���{�[�h�s���
	matBillboard = matCameraRot;

	constMap->matBillboard = matBillboard;


	ID3D12GraphicsCommandList* commandList = directX->GetCommandList();
	//h�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	//���f���f�[�^�̕`��p�R�}���h�̂܂Ƃ܂�
	//model->Draw(commandList);

	//���_�o�b�t�@�r���[�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);

	//�f�X�N���v�^�q�[�v�̔z��Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetDescHeap() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�n���h�����Z�b�g
	commandList->SetGraphicsRootDescriptorTable(1, texData->gpuSRVHandle);
	//�`��R�}���h
	commandList->DrawInstanced((UINT)std::distance(particles.begin(),particles.end()), 1, 0, 0);
 }

void ParticleManager::Add(int life, const Vector3& position, const Vector3& velocity, const Vector3& accel, float scaleStart, float scaleEnd) {
	//�p�[�e�B�N���̐����ő�Ȃ�ǉ����Ȃ�
	if (std::distance(particles.begin(), particles.end()) >= vertexCount) {
		return;
	}
	
	//���X�g�ɗv�f��ǉ�
	particles.emplace_front();
	//�ǉ������v�f�̎Q��
	Particle& p = particles.front();
	//�l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.scale = scaleStart;
	p.scaleStart = scaleStart;
	p.scaleEnd = scaleEnd;
}

void ParticleManager::AddLerp(float t, const Vector3& start, const Vector3& end, float scaleStart, float scaleEnd)
{//�p�[�e�B�N���̐����ő�Ȃ�ǉ����Ȃ�
	if (std::distance(particles.begin(), particles.end()) >= vertexCount) {
		return;
	}

	//���X�g�ɗv�f��ǉ�
	particles.emplace_front();
	//�ǉ������v�f�̎Q��
	Particle& p = particles.front();
	p.start = start;
	p.end = end;
	p.scaleStart = scaleStart;
	p.scaleEnd = scaleEnd;
}

void ParticleManager::CreatePipeline3D()
{
	HRESULT result;

	ID3D12Device* dev = directX->GetDevice();

	ComPtr<ID3DBlob> vsBlob;		// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob;		// �W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;		// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleGS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticlePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//�p�C�v���C���ݒ�
	GpPipeline pipeline3D;
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	inputLayout.push_back(
		{
		"POSITION",										//�Z�}���e�B�b�N��
		0,												//�����Z�}���e�B�b�N�����������鎞�Ɏg���C���f�b�N�X�i0�ł悢�j
		DXGI_FORMAT_R32G32B32_FLOAT,					//�v�f���ƃr�b�g����\���iXYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT�j
		0,												//���̓X���b�g�C���f�b�N�X(0�ł悢)
		D3D12_APPEND_ALIGNED_ELEMENT,					//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		//���̓f�[�^���(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0												//��x�ɕ`�悷��C���X�^���X��(0�ł悢)
		});

	inputLayout.push_back(
		{
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});

	pipeline3D.SetPipeline(vsBlob.Get(), psBlob.Get(), inputLayout);
	//�W�I���g���V�F�[�_�[�̐ݒ��ǉ�
	pipeline3D.desc.GS.BytecodeLength = gsBlob->GetBufferSize();
	pipeline3D.desc.GS.pShaderBytecode = gsBlob->GetBufferPointer();

	pipeline3D.desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	//�f�v�X�̏������݋֎~
	pipeline3D.desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	pipeline3D.SetBlendAdd();
	//pipeline3D.SetBlendSub();

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;		//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��(�r���{�[�h�s��̒萔)
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_���猩����
	//�萔�o�b�t�@1��(�J�����s��)
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�f�X�N���v�^�����W
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�X�N���v�^�����W��
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����o�b�t�@

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;				//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���ׂă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipeline3D.desc.pRootSignature = rootSignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	pipeline3D.SetPipelineState(dev, pipelineState);
}

