#include "Affine.h"

//�~����
const float PI = 3.1415926535f;

const float PI2 = PI * 2;

using namespace std;

//�P�ʍs��
void IdentityMatrix(Matrix4& matrix) {
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
}

#pragma region �A�t�B���ϊ�����

//�X�P�[�����O�s�񐶐�
Matrix4 ScalingForm(float scalx, float scaly, float scalz) {
	//�X�P�[�����O�s���錾
	Matrix4 matScale;

	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = scalx;
	matScale.m[1][1] = scaly;
	matScale.m[2][2] = scalz;
	matScale.m[3][3] = 1.0f;

	return matScale;
}

// X����]�s��𐶐�
Matrix4 RotationXForm(float angle) {
	// X����]�s���錾
	Matrix4 matRotX;

	// X����]�s��̊e�v�f��ݒ肷��
	matRotX.m[1][1] = cos(angle);
	matRotX.m[1][2] = sin(angle);

	matRotX.m[2][1] = -sin(angle);
	matRotX.m[2][2] = cos(angle);

	matRotX.m[0][0] = 1.0f;
	;
	matRotX.m[3][3] = 1.0f;

	return matRotX;
}

// Z����]�s��𐶐�
Matrix4 RotationZForm(float angle) {
	// Z����]�s���錾
	Matrix4 matRotZ;

	// Z����]�s��̊e�v�f��ݒ肷��
	matRotZ.m[0][0] = cos(angle);
	matRotZ.m[0][1] = sin(angle);

	matRotZ.m[1][1] = -sin(angle);
	matRotZ.m[1][0] = cos(angle);

	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	return matRotZ;
}

// Y����]�s��𐶐�
Matrix4 RotationYForm(float angle) {
	// Y����]�s���錾
	Matrix4 matRotY;

	// Y����]�s��̊e�v�f��ݒ肷��
	matRotY.m[0][0] = cos(angle);
	matRotY.m[0][2] = -sin(angle);

	matRotY.m[2][0] = sin(angle);
	matRotY.m[2][2] = cos(angle);

	matRotY.m[1][1] = 1.0f;
	matRotY.m[3][3] = 1.0f;

	return matRotY;
}

//��]�s��𐶐�
Matrix4 Rotation(float xangle, float yangle, float zangle) {
	Matrix4 matRot;

	//�P�ʍs�����
	IdentityMatrix(matRot);

	// matWorld_��Z����]�s����|���Z
	matRot *= RotationZForm(zangle);

	// matWorld_��X����]�s����|���Z
	matRot *= RotationXForm(xangle);

	// matWorld_��Y����]�s����|���Z
	matRot *= RotationXForm(yangle);

	return matRot;
}

//���s�ړ��s��𐶐�
Matrix4 TransferForm(float x, float y, float z) {
	//���s�ړ��s���錾
	Matrix4 matTrans;
	//�P�ʍs�����
	IdentityMatrix(matTrans);

	//�ړ��ʂ��s��ɐݒ肷��
	matTrans.m[3][0] = x;
	matTrans.m[3][1] = y;
	matTrans.m[3][2] = z;

	return matTrans;
}

//���[���h�s��𐶐�
Matrix4 WorldForm(Matrix4& scale, Matrix4& rotx, Matrix4& roty, Matrix4& rotz, Matrix4& trans) {
	Matrix4 matWorld;

	//�P�ʍs�����
	IdentityMatrix(matWorld);

	// matWorld_�ɃX�P�[�����O�{�����|���Z
	matWorld *= scale;

	// matWorld_��Z����]�s����|���Z
		matWorld *= rotz;

	// matWorld_��X����]�s����|���Z
		matWorld *= rotx;

	// matWorld_��Y����]�s����|���Z
		matWorld *= roty;

	// matWorld_�Ɉړ��ʂ��|���Z
	matWorld *= trans;

	return matWorld;
}

#pragma endregion

#pragma region �A�t�B���ϊ�

//�A�t�B���ϊ�������
void AffineTransformation(WorldTransform& worldtransform) {

	//�X�P�[�����O�s���錾
	Matrix4 matScale;

	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = worldtransform.scale_.x;
	matScale.m[1][1] = worldtransform.scale_.y;
	matScale.m[2][2] = worldtransform.scale_.z;
	matScale.m[3][3] = 1.0f;

	// Z����]�s���錾
	Matrix4 matRotZ;
	// Z����]�s��̊e�v�f��ݒ肷��
	matRotZ.m[0][0] = cos(worldtransform.rotation_.z);
	matRotZ.m[0][1] = -sin(worldtransform.rotation_.z);

	matRotZ.m[1][0] = sin(worldtransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldtransform.rotation_.z);

	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	// X����]�s���錾
	Matrix4 matRotX;
	// X����]�s��̊e�v�f��ݒ肷��
	matRotX.m[1][1] = cos(worldtransform.rotation_.x);
	matRotX.m[1][2] = sin(worldtransform.rotation_.x);

	matRotX.m[2][1] = -sin(worldtransform.rotation_.x);
	matRotX.m[2][2] = cos(worldtransform.rotation_.x);

	matRotX.m[0][0] = 1.0f;
	matRotX.m[3][3] = 1.0f;

	// Y����]�s���錾
	Matrix4 matRotY;

	// Y����]�s��̊e�v�f��ݒ肷��
	matRotY.m[0][0] = cos(worldtransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldtransform.rotation_.y);

	matRotY.m[2][0] = sin(worldtransform.rotation_.y);
	matRotY.m[2][2] = cos(worldtransform.rotation_.y);

	matRotY.m[1][1] = 1.0f;
	matRotY.m[3][3] = 1.0f;

	//���s�ړ��s���錾
	Matrix4 matTrans;
	//�P�ʍs�����
	IdentityMatrix(matTrans);

	//�ړ��ʂ��s��ɐݒ肷��
	matTrans.m[3][0] = worldtransform.translation_.x;
	matTrans.m[3][1] = worldtransform.translation_.y;
	matTrans.m[3][2] = worldtransform.translation_.z;

	//�P�ʍs�����
	IdentityMatrix(worldtransform.matWorld_);

	// matWorld_�ɃX�P�[�����O�{�����|���Z
	worldtransform.matWorld_ *= matScale;

	// matWorld_��Z����]�s����|���Z
	worldtransform.matWorld_ *= matRotZ;

	// matWorld_��X����]�s����|���Z
	worldtransform.matWorld_ *= matRotX;

	// matWorld_��Y����]�s����|���Z
	worldtransform.matWorld_ *= matRotY;

	// matWorld_�Ɉړ��ʂ��|���Z
	worldtransform.matWorld_ *= matTrans;

	//�s��̓]��
	worldtransform.TransferMatrix();
}

//�A�t�B���ϊ��֐�
void AffineTransformationFunction(WorldTransform& worldtransform) {

	//�X�P�[�����O�s���錾
	Matrix4 matScale;

	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale = MathUtility::Matrix4Scaling(
	  worldtransform.scale_.x, worldtransform.scale_.y, worldtransform.scale_.z);

	// Z����]�s���錾
	Matrix4 matRotZ;
	// Z����]�s��̊e�v�f��ݒ肷��
	if (worldtransform.rotation_.z != 0) {
		matRotZ = MathUtility::Matrix4RotationZ(worldtransform.rotation_.z);
	}

	// X����]�s���錾
	Matrix4 matRotX;
	// X����]�s��̊e�v�f��ݒ肷��
	if (worldtransform.rotation_.x != 0) {
		matRotX = MathUtility::Matrix4RotationX(worldtransform.rotation_.x);
	}
	// Y����]�s���錾
	Matrix4 matRotY;
	// Y����]�s��̊e�v�f��ݒ肷��
	if (worldtransform.rotation_.y != 0) {
		matRotY = MathUtility::Matrix4RotationY(worldtransform.rotation_.y);
	}

	//���s�ړ��s���錾�E�P�ʍs�����
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//�ړ��ʂ��s��ɐݒ肷��
	matTrans = MathUtility::Matrix4Translation(
	  worldtransform.translation_.x, worldtransform.translation_.y, worldtransform.translation_.z);

	//�P�ʍs�����
	worldtransform.matWorld_ = MathUtility::Matrix4Identity();

	// matWorld_�ɃX�P�[�����O�{�����|���Z
	worldtransform.matWorld_ *= matScale;

	// matWorld_��Z����]�s����|���Z
	if (worldtransform.rotation_.z != 0.0f) {
		worldtransform.matWorld_ *= matRotZ;
	}
	// matWorld_��X����]�s����|���Z
	if (worldtransform.rotation_.x != 0.0f) {
		worldtransform.matWorld_ *= matRotX;
	}
	// matWorld_��Y����]�s����|���Z
	if (worldtransform.rotation_.y != 0.0f) {
		worldtransform.matWorld_ *= matRotY;
	}
	// matWorld_�Ɉړ��ʂ��|���Z
	worldtransform.matWorld_ *= matTrans;

	//�s��̓]��
	worldtransform.TransferMatrix();
}

#pragma endregion
