#pragma once
#include"DirectX.h"
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include"Matrix4.h"

class Camera
{
public:
	static ID3D12Device* device;

	static void StaticInitialize(ID3D12Device* dev);

	struct ConstBufferCamera {
		Matrix4 view;
		Matrix4 projection;
	};

	ConstBufferCamera* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	Matrix4 matView;
	Matrix4 matProjection;
	Vector3 eye = {0,0,0};
	Vector3 target = { 0,0,10 };
	Vector3 up = { 0,1,0 };
	float nearZ = 0.1f;
	float farZ = 1000.0f;

public:


	void Initialize(const Vector3& eye_, const Vector3& target_, const Vector3& up_);

	void UpdateMatrix();

	Matrix4 GetViewProjection()const { return matView * matProjection; }

	Vector3 GetEye()const { return eye; }

	Matrix4 GetView()const { return matView; }
		 
};

