#pragma once
#include <cassert>

#include "Model.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Collider.h"
#include "MyMath.h"

/// <summary>
///�@���L�����̒e
/// </summary>
class PlayerBullet : public Collider {
  public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W </param>
	/// /// <param name="velocity">���x </param>
	void Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// /// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection viewProjection);

	///�f�X�t���O�Q�b�^�[
	bool IsDead() const;

	/// <summary>
	/// ���a������
	/// </summary>
	float GetRadius();

	//�Փˎ��ɌĂ΂��֐�
	void OnCollision() override;

	/// <summary>
	/// ���[���h���W������
	/// </summary>
	Vector3 GetWorldPosition() override;

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	std::shared_ptr<Model> model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t texturehandle_ = 0u;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

	//���a
	const float radius_ = 1.0f;
};

inline bool PlayerBullet::IsDead() const { return isDead_; }
