#pragma once
#include<memory>
#include<list>

#include"Model.h"
#include"WorldTransform.h"
#include"Input.h"
#include"DebugText.h"

#include"PlayerBullet.h"

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

	//�L�[�{�[�h����
	Input* input_ = nullptr;

	//�f�o�b�O�p�\��
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ����
	/// </summary>
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();
};

