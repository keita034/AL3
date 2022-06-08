#include "Player.h"

#include "ViewProjection.h"

#include <DirectXMath.h>
#include <cassert>

/// <summary>
/// �ړ�
/// </summary>
void Player::Move() {
	//�L�����N�^�[�ړ�����
	{
		//�L�����N�^�[�ړ��x�N�g��
		Vector3 move = {0, 0, 0};

		const float kCharacterSpeed = 0.5f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		worldTransform_.translation_ += move;
	}

	{
		//�L�����N�^�[�ړ��x�N�g��
		Vector3 move = {0, 0, 0};

		const float kCharacterSpeed = 0.5f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_UP)) {
			move = {0, kCharacterSpeed, 0};
		} else if (input_->PushKey(DIK_DOWN)) {
			move = {0, -kCharacterSpeed, 0};
		}

		worldTransform_.translation_ += move;
	}

	//�ړ�����
	{
		const float kMoveLimitX = 35.0f;
		const float kMoveLimitY = 19.0f;

		worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
	}
}

/// <summary>
/// ����
/// </summary>
void Player::Rotate() {

	//��]����
	{
		//��]�̑���
		const float kChestaRotSpeed = 0.07f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_F)) {
			worldTransform_.rotation_.y -= kChestaRotSpeed;
		} else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += kChestaRotSpeed;
		}
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//���L�����̍��W���R�s�[
		Vector3 position = worldTransform_.translation_;

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = worldTransform_.VecMatMul(velocity);

		// �e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="textureHandle">�e�N�X�`���n���h��</param>
void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NUUL�|�C���^�v�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	texturehandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//�ړ�
	Move();

	//����
	Rotate();

	//���[���h�s��v�Z
	worldTransform_.AffineTransformation();

	//�U��
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�f�o�b�O�p�\��
	debugText_->SetPos(0, 0);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf("Rot:%f", worldTransform_.rotation_.y);
}

/// <summary>
/// �`��
/// </summary>
/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
