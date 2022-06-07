#include "WorldTransform.h"

//�X�P�[�����O�s�񐶐�
Matrix4 WorldTransform::ScalingForm() {
	Matrix4 Scale;

	//�X�P�[�����O�{�����s��ɐݒ肷��
	Scale.m[0][0] = scale_.x;
	Scale.m[1][1] = scale_.y;
	Scale.m[2][2] = scale_.z;
	Scale.m[3][3] = 1.0f;

	return Scale;
}

// X����]�s��𐶐�
Matrix4 WorldTransform::RotationXForm() {
	Matrix4 RotX;

	// X����]�s��̊e�v�f��ݒ肷��
	RotX.m[1][1] = std::cos(rotation_.x);
	RotX.m[1][2] = std::sin(rotation_.x);

	RotX.m[2][1] = -std::sin(rotation_.x);
	RotX.m[2][2] = std::cos(rotation_.x);

	RotX.m[0][0] = 1.0f;

	RotX.m[3][3] = 1.0f;

	return RotX;
}

// Y����]�s��𐶐�
Matrix4 WorldTransform::RotationYForm() {
	Matrix4 RotY;

	// Y����]�s��̊e�v�f��ݒ肷��
	RotY.m[0][0] = std::cos(rotation_.y);
	RotY.m[0][2] = -std::sin(rotation_.y);

	RotY.m[2][0] = std::sin(rotation_.y);
	RotY.m[2][2] = std::cos(rotation_.y);

	RotY.m[1][1] = 1.0f;
	RotY.m[3][3] = 1.0f;

	return RotY;
}

// Z����]�s��𐶐�
Matrix4 WorldTransform::RotationZForm() {
	Matrix4 RotZ;

	// Z����]�s��̊e�v�f��ݒ肷��
	RotZ.m[0][0] = std::cos(rotation_.z);
	RotZ.m[0][1] = std::sin(rotation_.z);

	RotZ.m[1][0] = -std::sin(rotation_.z);
	RotZ.m[1][1] = std::cos(rotation_.z);

	RotZ.m[2][2] = 1.0f;
	RotZ.m[3][3] = 1.0f;

	return RotZ;
}

// ��]�s��𐶐�
Matrix4 WorldTransform::RotationForm() {
	Matrix4 Rot;

	//�P�ʍs�����
	Rot.IdentityMatrix();

	// matWorld_��Z����]�s����|���Z
	Rot *= RotationZForm();

	// matWorld_��X����]�s����|���Z
	Rot *= RotationXForm();

	// matWorld_��Y����]�s����|���Z
	Rot *= RotationYForm();

	return Rot;
}

//���s�ړ��s��𐶐�
Matrix4 WorldTransform::TransferForm() {
	//���s�ړ��s���錾
	Matrix4 Trans;
	//�P�ʍs�����
	Trans.IdentityMatrix();

	//�ړ��ʂ��s��ɐݒ肷��
	Trans.m[3][0] = translation_.x;
	Trans.m[3][1] = translation_.y;
	Trans.m[3][2] = translation_.z;

	return Trans;
}

//���[���h�s��𐶐�
Matrix4 WorldTransform::WorldForm() {
	Matrix4 World;

	//�P�ʍs�����
	World.IdentityMatrix();

	// matWorld_�ɃX�P�[�����O�{�����|���Z
	World *= ScalingForm();

	// matWorld_��Z����]�s����|���Z
	World *= RotationZForm();

	// matWorld_��X����]�s����|���Z
	World *= RotationXForm();

	// matWorld_��Y����]�s����|���Z
	World *= RotationYForm();

	// matWorld_�Ɉړ��ʂ��|���Z
	World *= TransferForm();

	return World;
}

//�A�t�B���ϊ�
void WorldTransform::AffineTransformation() {
	//�P�ʍs�����
	matWorld_.IdentityMatrix();

	// matWorld_�ɃX�P�[�����O�{�����|���Z
	matWorld_ *= ScalingForm();

	// matWorld_��Z����]�s����|���Z
	matWorld_ *= RotationZForm();

	// matWorld_��X����]�s����|���Z
	matWorld_ *= RotationXForm();

	// matWorld_��Y����]�s����|���Z
	matWorld_ *= RotationYForm();

	// matWorld_�Ɉړ��ʂ��|���Z
	matWorld_ *= TransferForm();

	//�s��̓]��
	TransferMatrix();
}

void WorldTransform::ParenChildUpdate()
{
	matWorld_ = WorldForm();

	matWorld_ *= parent_->matWorld_;

	TransferMatrix();
}