#pragma once


#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"DebugText.h"


/// <summary>
/// ���L����
/// </summary>
class Player
{
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

	Player() = default;
	~Player()= default;

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t texturehandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();
};

