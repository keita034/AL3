#pragma once
#include "GameScene.h"

//単位行列
Matrix4 IdentityMatrix();

//スケーリング行列生成
Matrix4 ScalingForm(float scalx, float scaly, float scalz);

// X軸回転行列を生成
Matrix4 RotationXForm(float angle);

// Z軸回転行列を生成
Matrix4 RotationZForm(float angle);

// Y軸回転行列を生成
Matrix4 RotationYForm(float angle);

//回転行列を生成
Matrix4 Rotation(float xangle, float yangle, float zangle);

//平行移動行列を生成
Matrix4 TransferForm(float x, float y, float z);

//ワールド行列を生成
Matrix4 WorldForm(Matrix4& scale, Matrix4& rotx, Matrix4& roty, Matrix4& rotz, Matrix4& trans);

Matrix4 WorldForm(WorldTransform& worldtransform);

//アフィン変換自分で
void AffineTransformation(WorldTransform& worldtransform);

//アフィン変換関数
void AffineTransformationFunction(WorldTransform& worldtransform);