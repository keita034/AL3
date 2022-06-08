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

	//�ڋ߃t�F�[�Y������
	approachPhaseInt();
}

/// <summary>
/// �X�V����
/// </summary>
void Enemy::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

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
	worldTransform_.AffineTransformation();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

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

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

/// <summary>
/// �e����
/// </summary>
void Enemy::Fire() {
	//���L�����̍��W���R�s�[
	Vector3 position = worldTransform_.translation_;

	//�e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = worldTransform_.VecMatMul(velocity);

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, position, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

//�ڋ߃t�F�[�Y������
void Enemy::approachPhaseInt()
{
	//���˃^�C�}�[��������
	fileTimer_ = kFireInterval;
}

// �ڋ߃t�F�[�Y�ړ�����
void Enemy::ApproachVelocity() {

	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer_--;
	//�w�莞�ԂɒB����
	if (fileTimer_==0)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer_ = kFireInterval;
	}

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
