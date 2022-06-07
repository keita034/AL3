#include "WorldTransform.h"

//ƒXƒP[ƒŠƒ“ƒOs—ñ¶¬
Matrix4 WorldTransform::ScalingForm() {
	Matrix4 Scale;

	//ƒXƒP[ƒŠƒ“ƒO”{—¦‚ðs—ñ‚ÉÝ’è‚·‚é
	Scale.m[0][0] = scale_.x;
	Scale.m[1][1] = scale_.y;
	Scale.m[2][2] = scale_.z;
	Scale.m[3][3] = 1.0f;

	return Scale;
}

// XŽ²‰ñ“]s—ñ‚ð¶¬
Matrix4 WorldTransform::RotationXForm() {
	Matrix4 RotX;

	// XŽ²‰ñ“]s—ñ‚ÌŠe—v‘f‚ðÝ’è‚·‚é
	RotX.m[1][1] = std::cos(rotation_.x);
	RotX.m[1][2] = std::sin(rotation_.x);

	RotX.m[2][1] = -std::sin(rotation_.x);
	RotX.m[2][2] = std::cos(rotation_.x);

	RotX.m[0][0] = 1.0f;

	RotX.m[3][3] = 1.0f;

	return RotX;
}

// YŽ²‰ñ“]s—ñ‚ð¶¬
Matrix4 WorldTransform::RotationYForm() {
	Matrix4 RotY;

	// YŽ²‰ñ“]s—ñ‚ÌŠe—v‘f‚ðÝ’è‚·‚é
	RotY.m[0][0] = std::cos(rotation_.y);
	RotY.m[0][2] = -std::sin(rotation_.y);

	RotY.m[2][0] = std::sin(rotation_.y);
	RotY.m[2][2] = std::cos(rotation_.y);

	RotY.m[1][1] = 1.0f;
	RotY.m[3][3] = 1.0f;

	return RotY;
}

// ZŽ²‰ñ“]s—ñ‚ð¶¬
Matrix4 WorldTransform::RotationZForm() {
	Matrix4 RotZ;

	// ZŽ²‰ñ“]s—ñ‚ÌŠe—v‘f‚ðÝ’è‚·‚é
	RotZ.m[0][0] = std::cos(rotation_.z);
	RotZ.m[0][1] = std::sin(rotation_.z);

	RotZ.m[1][0] = -std::sin(rotation_.z);
	RotZ.m[1][1] = std::cos(rotation_.z);

	RotZ.m[2][2] = 1.0f;
	RotZ.m[3][3] = 1.0f;

	return RotZ;
}

// ‰ñ“]s—ñ‚ð¶¬
Matrix4 WorldTransform::RotationForm() {
	Matrix4 Rot;

	//’PˆÊs—ñ‚ð‘ã“ü
	Rot.IdentityMatrix();

	// matWorld_‚ÉZŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	Rot *= RotationZForm();

	// matWorld_‚ÉXŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	Rot *= RotationXForm();

	// matWorld_‚ÉYŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	Rot *= RotationYForm();

	return Rot;
}

//•½sˆÚ“®s—ñ‚ð¶¬
Matrix4 WorldTransform::TransferForm() {
	//•½sˆÚ“®s—ñ‚ðéŒ¾
	Matrix4 Trans;
	//’PˆÊs—ñ‚ð‘ã“ü
	Trans.IdentityMatrix();

	//ˆÚ“®—Ê‚ðs—ñ‚ÉÝ’è‚·‚é
	Trans.m[3][0] = translation_.x;
	Trans.m[3][1] = translation_.y;
	Trans.m[3][2] = translation_.z;

	return Trans;
}

//ƒ[ƒ‹ƒhs—ñ‚ð¶¬
Matrix4 WorldTransform::WorldForm() {
	Matrix4 World;

	//’PˆÊs—ñ‚ð‘ã“ü
	World.IdentityMatrix();

	// matWorld_‚ÉƒXƒP[ƒŠƒ“ƒO”{—¦‚ðŠ|‚¯ŽZ
	World *= ScalingForm();

	// matWorld_‚ÉZŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	World *= RotationZForm();

	// matWorld_‚ÉXŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	World *= RotationXForm();

	// matWorld_‚ÉYŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	World *= RotationYForm();

	// matWorld_‚ÉˆÚ“®—Ê‚ðŠ|‚¯ŽZ
	World *= TransferForm();

	return World;
}

//ƒAƒtƒBƒ“•ÏŠ·
void WorldTransform::AffineTransformation() {
	//’PˆÊs—ñ‚ð‘ã“ü
	matWorld_.IdentityMatrix();

	// matWorld_‚ÉƒXƒP[ƒŠƒ“ƒO”{—¦‚ðŠ|‚¯ŽZ
	matWorld_ *= ScalingForm();

	// matWorld_‚ÉZŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	matWorld_ *= RotationZForm();

	// matWorld_‚ÉXŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	matWorld_ *= RotationXForm();

	// matWorld_‚ÉYŽ²‰ñ“]s—ñ‚ðŠ|‚¯ŽZ
	matWorld_ *= RotationYForm();

	// matWorld_‚ÉˆÚ“®—Ê‚ðŠ|‚¯ŽZ
	matWorld_ *= TransferForm();

	//s—ñ‚Ì“]‘—
	TransferMatrix();
}

void WorldTransform::ParenChildUpdate()
{
	matWorld_ = WorldForm();

	matWorld_ *= parent_->matWorld_;

	TransferMatrix();
}