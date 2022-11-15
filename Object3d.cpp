#include "Object3d.h"

#include"WindowsAPI.h"
#include<cassert>
using namespace DirectX;
using namespace Microsoft::WRL;
#include<d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"GpPipeline.h"
#include"Texture.h"
#include<fstream>
#include<sstream>

#include<vector>
using namespace std;

//�ÓI�����o�ϐ�
ComPtr<ID3D12PipelineState> Object3d::pipelineState;
ComPtr<ID3D12RootSignature> Object3d::rootSignature;
ReDirectX* Object3d::directX = nullptr;

Object3d::Object3d()
{
}

Object3d::~Object3d()
{
}

void Object3d::StaticInitialize(ReDirectX* directX_)
{
	//null�`�F�b�N
	assert(directX_);
	//�����o�ɓn��
	directX = directX_;
	//3D�p�p�C�v���C���X�e�[�g����
	CreatePipeline3D();
}

void Object3d::BeginDraw()
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	directX->GetCommandList()->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`���̐ݒ�
	directX->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`��̐ݒ�
	directX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::descHeap.Get() };
	directX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::descHeap->GetGPUDescriptorHandleForHeapStart();
	directX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void Object3d::CreateModel(std::string filename)
{
	HRESULT result;

	if (filename != "NULL") {
		HRESULT result = S_FALSE;

		//�t�@�C���X�g���[��
		ifstream file;
		//.obj�t�@�C�����J��
		file.open("Resources/triangle/triangle.obj");
		//�t�@�C���I�[�v�����s���`�F�b�N
		if (file.fail()) {
			assert(0);
		}

		vector<XMFLOAT3> positions;	//���_���W
		vector<XMFLOAT3> normals;	//�@���x�N�g��
		vector<XMFLOAT2> texcords;	//�e�N�X�`��UV
		//1�s���ǂݍ���
		string line;
		while (getline(file, line)) {

			//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
			istringstream line_stream(line);

			//���p�X�y�[�X��؂�ōs�̐擪��������擾
			string key;
			getline(line_stream, key, ' ');

			//�擪������v�Ȃ璸�_���W
			if (key == "v") {
				//X,Y,Z���W�ǂݍ���
				XMFLOAT3 position{};
				line_stream >> position.x;
				line_stream >> position.y;
				line_stream >> position.z;
				//���W�f�[�^�ɒǉ�
				positions.emplace_back(position);
				//���_�f�[�^�ɒǉ�
				Vertex vertex{};
				vertex.pos = position;
				vertices.emplace_back(vertex);
			}
			//�擪������f�Ȃ�|���S��(�O�p�`)
			if (key == "f") {
				//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
				string index_string;
				while (getline(line_stream, index_string, ' ')) {
					//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷���Ȃ�
					istringstream index_stream(index_string);
					unsigned short indexPosition;
					index_stream >> indexPosition;
					//���_�C���f�b�N�X�ɒǉ�
					indices.emplace_back(indexPosition - 1);
				}

			}
		}
		//�t�@�C�������
		file.close();

	}
	else {
		//���_�f�[�^�ƒ��_�o�b�t�@�r���[�̐���
		// ���_�f�[�^
		Vertex vertices_[] = {
			//     x     y    z     �@��  u    v

				//�O
				{{-5.0f,-5.0f,-5.0f}, {},{0.0f,1.0f}}, // ����
				{{-5.0f, 5.0f,-5.0f}, {},{0.0f,0.0f}}, // ����
				{{ 5.0f,-5.0f,-5.0f}, {},{1.0f,1.0f}}, // �E��
				{{ 5.0f, 5.0f,-5.0f}, {},{1.0f,0.0f}}, // �E��

				//��
				{{-5.0f,-5.0f, 5.0f}, {},{0.0f,1.0f}}, // ����
				{{-5.0f, 5.0f, 5.0f}, {},{0.0f,0.0f}}, // ����
				{{ 5.0f,-5.0f, 5.0f}, {},{1.0f,1.0f}}, // �E��
				{{ 5.0f, 5.0f, 5.0f}, {},{1.0f,0.0f}}, // �E��

				//��
				{{-5.0f,-5.0f,-5.0f}, {} ,{0.0f,1.0f}}, // ����
				{{-5.0f,-5.0f, 5.0f}, {} ,{0.0f,0.0f}}, // ����
				{{-5.0f, 5.0f,-5.0f}, {} ,{1.0f,1.0f}}, // �E��
				{{-5.0f, 5.0f, 5.0f}, {} ,{1.0f,0.0f}}, // �E��

				//�E
				{{ 5.0f,-5.0f,-5.0f}, {} ,{0.0f,1.0f}}, // ����
				{{ 5.0f,-5.0f, 5.0f}, {} ,{0.0f,0.0f}}, // ����
				{{ 5.0f, 5.0f,-5.0f}, {} ,{1.0f,1.0f}}, // �E��
				{{ 5.0f, 5.0f, 5.0f}, {} ,{1.0f,0.0f}}, // �E��

				//��
				{{-5.0f, 5.0f,-5.0f}, {},{0.0f,1.0f}}, // ����
				{{ 5.0f, 5.0f,-5.0f}, {},{0.0f,0.0f}}, // ����
				{{-5.0f, 5.0f, 5.0f}, {},{1.0f,1.0f}}, // �E��
				{{ 5.0f, 5.0f, 5.0f}, {},{1.0f,0.0f}}, // �E��

				//��
				{{-5.0f,-5.0f,-5.0f}, {},{0.0f,1.0f}}, // ����
				{{ 5.0f,-5.0f,-5.0f}, {},{0.0f,0.0f}}, // ����
				{{-5.0f,-5.0f, 5.0f}, {},{1.0f,1.0f}}, // �E��
				{{ 5.0f,-5.0f, 5.0f}, {},{1.0f,0.0f}}, // �E��
		};
		for (int i = 0; i < _countof(vertices_); i++) {
			vertices.push_back(vertices_[i]);
		}

		//�C���f�b�N�X�f�[�^
		unsigned short indices_[] = {

			//�O
			0,1,2,		//�O�p�`1��
			2,1,3,		//�O�p�`2��
			//��	
			5,4,7,		//�O�p�`3��
			7,4,6,		//�O�p�`4��
			//��
			8,9,10,		//�O�p�`5��
			10,9,11,	//�O�p�`6��
			//�E
			13,12,15,	//�O�p�`7��
			15,12,14,	//�O�p�`8��
			//��
			17,16,19,	//�O�p�`9��
			19,16,18,	//�O�p�`10��
			//��
			20,21,22,	//�O�p�`11��
			22,21,23,	//�O�p�`12��
		};

		for (int i = 0; i < _countof(indices_); i++) {
			indices.push_back(indices_[i]);
		}

	}



	//���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

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
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	



	//�@���̌v�Z
	for (int i = 0; i < indices.size() / 3; i++) {
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(indices[0]) * indices.size());

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i];//�C���f�b�N�X���R�s�[
	}
	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


}

void Object3d::Initialize()
{
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
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));

	//���f���̐���
	CreateModel("a");
	XMMATRIX matrix = XMMatrixIdentity();
	constMap->mat = matrix;

	//�F��]��
	constMap->color = XMFLOAT4(1,1,1,1);
}

void Object3d::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//�e�s��v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);	//Z������ɉ�]
	matRot *= XMMatrixRotationX(rotation.x);	//X������ɉ�]
	matRot *= XMMatrixRotationY(rotation.y);	//Y������ɉ�]
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();

	matWorld *= matScale;	//�X�P�[�����O�𔽉f
	matWorld *= matRot;	//��]�𔽉f
	matWorld *= matTrans;	//���s�ړ��𔽉f

	if (parent != nullptr) {
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@�ɓ]��
	constMap->mat = matWorld * matView * matProjection;
	constMap->color = color;

}

void Object3d::Draw()
{
	ID3D12GraphicsCommandList* commandList = directX->GetCommandList();

	//�v���~�e�B�u�`��̐ݒ�
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	commandList->IASetIndexBuffer(&ibView);
	//0�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	// �`��R�}���h
	commandList->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0);

}

void Object3d::CreatePipeline3D()
{
	HRESULT result;

	ID3D12Device* dev = directX->GetDevice();

	ComPtr<ID3DBlob> vsBlob;		// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;		// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // �V�F�[�_�t�@�C����
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


	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // �V�F�[�_�t�@�C����
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
		{//�@���x�N�g��
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});
	inputLayout.push_back({//uv���W
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		});

	pipeline3D.SetPipeline(vsBlob.Get(), psBlob.Get(), inputLayout);

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;		//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_���猩����
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�f�X�N���v�^�����W
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�X�N���v�^�����W��
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����o�b�t�@E

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
