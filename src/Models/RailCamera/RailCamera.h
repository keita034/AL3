#pragma once
#include "WorldTransform.h"
#include"ViewProjection.h"
#include"MyMath.h"
#include"MathUtility.h"
/// <summary>
/// ���[���J����
/// </summary>
class RailCamera
{
public:
	RailCamera();
	~RailCamera();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotaion);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Vector3& move, Vector3& rot);

	/// <summary>
	/// �r���[�v���W�F�N�V����������
	/// </summary>
	ViewProjection GetViewProjection();

	/// <summary>
	/// ���[���h�ϊ��f�[�^�̃A�h���X������
	/// </summary>
	WorldTransform* GetWorldTransformPtr();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
};

