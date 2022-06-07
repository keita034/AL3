#include "Enemy.h"
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

	worldTransform_.translation_ += velocity_;
	//���[���h�s��v�Z
	worldTransform_.AffineTransformation();

	//�f�o�b�O�p�\��
	debugText_->SetPos(0, 60);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

/// <summary>
/// �`��
/// </summary>
/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}