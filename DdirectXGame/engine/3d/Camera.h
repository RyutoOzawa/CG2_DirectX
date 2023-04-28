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
	Vector3 eye;
	Vector3 target;
	Vector3 up;

public:


	void Initialize(const Vector3& eye, const Vector3& target, const Vector3& up);

	void UpdateMatrix();

	Matrix4 GetViewProjection()const { return matView * matProjection; }

	Vector3 GetEye()const { return eye; }
		 
};

