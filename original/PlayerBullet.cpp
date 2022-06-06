#include "PlayerBullet.h"

#include <cassert>

#include "ViewProjection.h"
#include"TextureManager.h"

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W </param>
void PlayerBullet::Initialize(Model* model, const Vector3 position) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	//�e�N�X�`���ǂݍ���
	texturehandle_ = TextureManager::Load("black.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerBullet::Update() {
	//���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.AffineTransformation();
}

/// <summary>
/// �`��
/// </summary>
/// /// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
void PlayerBullet::Draw(const ViewProjection viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}
