#include "Enemy.h"
#include "Affine.h"
#include <cassert>

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="textureHandle">�e�N�X�`���n���h��</param>
void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NUUL�|�C���^�v�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	texturehandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Enemy::Update() {

	//�ړ�����
	switch (phase_) {
	case Phase::Approach: //�ڋ߃t�F�[�Y
		ApproachVelocity();
		break;
	case Phase::Leave: //���E�t�F�[�Y
		LeaveVelocity();
		break;
	}
	//���[���h�s��v�Z
	AffineTransformation(worldTransform_);

	//�f�o�b�O�p�\��
	debugText_->SetPos(0, 60);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(0, 80);
	if (phase_ == Phase::Approach) {
		debugText_->Printf("Phase:Approach");
	} else {
		debugText_->Printf("Phase:Leave");
	}
}

/// <summary>
/// �`��
/// </summary>
/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

// �ڋ߃t�F�[�Y�ړ�����
void Enemy::ApproachVelocity() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += approachVelocity_;
	//����̈ʒu�ɓ��������痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

//���E�t�F�[�Y�ړ�����
void Enemy::LeaveVelocity() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += leaveVelocity_;
}
