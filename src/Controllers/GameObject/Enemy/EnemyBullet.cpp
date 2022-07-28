#include "EnemyBullet.h"
#include"Player.h"

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W </param>
void EnemyBullet::Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity) {

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	velocity_ = velocity;

	//�e�N�X�`���ǂݍ���
	texturehandle_ = TextureManager::Load("images/red.png");

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//Z�����ɐL�т��`��
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	//���[���h�ϊ��̏�����
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	Vector3 temp = velocity;
	temp.y = 0.0f;
	worldTransform_.rotation_.x = std::atan2(-velocity.y, MyMath::Vector3Length(temp));
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

// �X�V
void EnemyBullet::Update() {
	//�G�e���玩�L�����ւ̃x�N�g�����v�Z
	Vector3 toPlayer =
		MyMath::Vector3Sub(player_->GetWorldPosition(), MyMath::GetWorldPosition(worldTransform_));

	//�x�N�g���𐳋K������
	MyMath::Vector3Normalize(toPlayer);
	MyMath::Vector3Normalize(velocity_);
	//���ʐ��`��Ԃɂ��A���̑��x�Ǝ��L�����ւ̃x�N�g������}���A�V���ȑ��x�Ƃ���
	velocity_ = MyMath::Vector3Mul(MyMath::Slerp(velocity_, toPlayer, 0.01f), 1.0f);

	//���W���ړ�������(1�t���[�����̈ړ�����������)
	worldTransform_.translation_ += velocity_;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 temp = velocity_;
	temp.y = 0.0f;
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, MyMath::Vector3Length(temp));

	//���[���h�g�����X�t�H�[���̍X�V
	MyMath::AffineTransformation(worldTransform_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

// �`��
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void EnemyBullet::OnCollision() {
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

//���[���h���W���擾
Vector3 EnemyBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

float EnemyBullet::GetRadius() { return radius_; }