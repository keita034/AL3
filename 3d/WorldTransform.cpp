#include "WorldTransform.h"

//スケーリング行列生成
Matrix4 WorldTransform::ScalingForm() {
	Matrix4 Scale;

	//スケーリング倍率を行列に設定する
	Scale.m[0][0] = scale_.x;
	Scale.m[1][1] = scale_.y;
	Scale.m[2][2] = scale_.z;
	Scale.m[3][3] = 1.0f;

	return Scale;
}

// X軸回転行列を生成
Matrix4 WorldTransform::RotationXForm() {
	Matrix4 RotX;

	// X軸回転行列の各要素を設定する
	RotX.m[1][1] = std::cos(rotation_.x);
	RotX.m[1][2] = std::sin(rotation_.x);

	RotX.m[2][1] = -std::sin(rotation_.x);
	RotX.m[2][2] = std::cos(rotation_.x);

	RotX.m[0][0] = 1.0f;

	RotX.m[3][3] = 1.0f;

	return RotX;
}

// Y軸回転行列を生成
Matrix4 WorldTransform::RotationYForm() {
	Matrix4 RotY;

	// Y軸回転行列の各要素を設定する
	RotY.m[0][0] = std::cos(rotation_.y);
	RotY.m[0][2] = -std::sin(rotation_.y);

	RotY.m[2][0] = std::sin(rotation_.y);
	RotY.m[2][2] = std::cos(rotation_.y);

	RotY.m[1][1] = 1.0f;
	RotY.m[3][3] = 1.0f;

	return RotY;
}

// Z軸回転行列を生成
Matrix4 WorldTransform::RotationZForm() {
	Matrix4 RotZ;

	// Z軸回転行列の各要素を設定する
	RotZ.m[0][0] = std::cos(rotation_.z);
	RotZ.m[0][1] = std::sin(rotation_.z);

	RotZ.m[1][0] = -std::sin(rotation_.z);
	RotZ.m[1][1] = std::cos(rotation_.z);

	RotZ.m[2][2] = 1.0f;
	RotZ.m[3][3] = 1.0f;

	return RotZ;
}

// 回転行列を生成
Matrix4 WorldTransform::RotationForm() {
	Matrix4 Rot;

	//単位行列を代入
	Rot.IdentityMatrix();

	// matWorld_にZ軸回転行列を掛け算
	Rot *= RotationZForm();

	// matWorld_にX軸回転行列を掛け算
	Rot *= RotationXForm();

	// matWorld_にY軸回転行列を掛け算
	Rot *= RotationYForm();

	return Rot;
}

//平行移動行列を生成
Matrix4 WorldTransform::TransferForm() {
	//平行移動行列を宣言
	Matrix4 Trans;
	//単位行列を代入
	Trans.IdentityMatrix();

	//移動量を行列に設定する
	Trans.m[3][0] = translation_.x;
	Trans.m[3][1] = translation_.y;
	Trans.m[3][2] = translation_.z;

	return Trans;
}

//ワールド行列を生成
Matrix4 WorldTransform::WorldForm() {
	Matrix4 World;

	//単位行列を代入
	World.IdentityMatrix();

	// matWorld_にスケーリング倍率を掛け算
	World *= ScalingForm();

	// matWorld_にZ軸回転行列を掛け算
	World *= RotationZForm();

	// matWorld_にX軸回転行列を掛け算
	World *= RotationXForm();

	// matWorld_にY軸回転行列を掛け算
	World *= RotationYForm();

	// matWorld_に移動量を掛け算
	World *= TransferForm();

	return World;
}

//アフィン変換
void WorldTransform::AffineTransformation() {
	//単位行列を代入
	matWorld_.IdentityMatrix();

	// matWorld_にスケーリング倍率を掛け算
	matWorld_ *= ScalingForm();

	// matWorld_にZ軸回転行列を掛け算
	matWorld_ *= RotationZForm();

	// matWorld_にX軸回転行列を掛け算
	matWorld_ *= RotationXForm();

	// matWorld_にY軸回転行列を掛け算
	matWorld_ *= RotationYForm();

	// matWorld_に移動量を掛け算
	matWorld_ *= TransferForm();

	//行列の転送
	TransferMatrix();
}