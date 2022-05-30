#pragma once
#include "GameScene.h"

//�P�ʍs��
Matrix4 IdentityMatrix();

//�X�P�[�����O�s�񐶐�
Matrix4 ScalingForm(float scalx, float scaly, float scalz);

// X����]�s��𐶐�
Matrix4 RotationXForm(float angle);

// Z����]�s��𐶐�
Matrix4 RotationZForm(float angle);

// Y����]�s��𐶐�
Matrix4 RotationYForm(float angle);

//��]�s��𐶐�
Matrix4 Rotation(float xangle, float yangle, float zangle);

//���s�ړ��s��𐶐�
Matrix4 TransferForm(float x, float y, float z);

//���[���h�s��𐶐�
Matrix4 WorldForm(Matrix4& scale, Matrix4& rotx, Matrix4& roty, Matrix4& rotz, Matrix4& trans);

Matrix4 WorldForm(WorldTransform& worldtransform);

//�A�t�B���ϊ�������
void AffineTransformation(WorldTransform& worldtransform);

//�A�t�B���ϊ��֐�
void AffineTransformationFunction(WorldTransform& worldtransform);