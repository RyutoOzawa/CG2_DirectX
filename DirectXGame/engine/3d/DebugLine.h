#pragma once
#include "Vector3.h"
#include"Vector4.h"
#include"Camera.h"
#include"DirectX.h"
#include<vector>

class DebugLine
{
public://�ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ReDirectX* directX);

	//�`��O����	
	static void BeginDraw(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="points">���W�z��</param>
	/// <param name="color">�F</param>
	static void Draw(std::vector<Vector3> points,const Vector4& color);

public://�ÓI�����o�ϐ��ƃT�u�N���X

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	static ReDirectX* directX;
	static Camera* camera;

	struct VertexPos {
		Vector3 pos;
	};

	struct ConstBufferData {
		Matrix4 mat;
		Vector4 color;
	};


private:

	/// <summary>
	/// �p�C�v���C���ݒ�
	/// </summary>
	static void CreatePipeline();

};
