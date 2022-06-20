#include "PlayerBullet.h"

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W </param>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	velocity_ = velocity;

	//�e�N�X�`���ǂݍ���
	texturehandle_ = TextureManager::Load("black.png");

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void PlayerBullet::Update() {

	//���W���ړ�������(1�t���[�����̈ړ�����������)
	worldTransform_.translation_ += velocity_;

	//���[���h�g�����X�t�H�[���̍X�V
	MyMath::AffineTransformation(worldTransform_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

/// <summary>
/// �`��
/// </summary>
/// /// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
void PlayerBullet::Draw(const ViewProjection viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void PlayerBullet::OnCollision() {
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

//���[���h���W���擾
Vector3 PlayerBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

float PlayerBullet::GetRadius() { return radius_; }