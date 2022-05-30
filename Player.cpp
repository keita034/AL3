#include "Player.h"


#include"ViewProjection.h"
#include"Affine.h"
#include<cassert>

void Player::Move() {
	//�L�����N�^�[�ړ�����
	{
		//�L�����N�^�[�ړ�����
		Vector3 move = {0, 0, 0};
		//�L�����N�^�[�ړ��x�N�g��
		const float kCharacterSpeed = 0.2f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		worldTransform_.translation_ += move;
	}

	{
		//�L�����N�^�[�ړ�����
		Vector3 move = {0, 0, 0};
		//�L�����N�^�[�ړ��x�N�g��
		const float kCharacterSpeed = 0.2f;

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

		worldTransform_.translation_.x = max(worldTransform_.translation_.x,-kMoveLimitX);
		worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	}
	
	AffineTransformation(worldTransform_);
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
	Move();

	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

/// <summary>
/// �`��
/// </summary>
/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

