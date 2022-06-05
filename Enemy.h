#pragma once
#include "DebugText.h"
#include "Model.h"
#include "ViewProjection.h"
#include "input.h"

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave,    //���E����

};

/// <summary>
///�G
/// </summary>
class Enemy {
  public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

  private:
	/// <summary>
	/// �ڋ߃t�F�[�Y�ړ�����
	/// </summary>
	void ApproachVelocity();

	/// <summary>
	/// ���E�t�F�[�Y�ړ�����
	/// </summary>
	void LeaveVelocity();

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t texturehandle_ = 0u;

	//�L�[�{�[�h����
	Input* input_ = nullptr;

	//�f�o�b�O�p�\��
	DebugText* debugText_ = nullptr;

	//���x
	Vector3 approachVelocity_ = {0, 0, -0.3f};
	Vector3 leaveVelocity_ = {-0.1f, 0.1f, -0.1f};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
};
