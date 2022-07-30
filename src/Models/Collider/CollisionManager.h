#pragma once
#include <cassert>
#include <list>
#include <memory>

#include "Collider.h"
#include "MyMath.h"
#include "PlayerBullet.h"

class CollisionManager {
  private:
	//�R���C�_�[���X�g
	std::list<Collider*> colliders_;

  public:
	/// <summary>
	/// �R���C�_�[�����X�g�ɓo�^
	/// </summary>
	void ColliderSet(Collider* collider);

	/// <summary>
	/// �R���C�_�[���X�g���N���A
	/// </summary>
	void ListClear();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary
	void CheckCollisions();

  private:
	/// <summary>
	/// ���Ƌ��̏Փ˔���Ɖ���
	/// </summary>
	/// <param name="colliderA">�R���C�_�[A</param>
	/// <param name="colliderB">�R���C�_�[B</param>
	void CheckSphereToSphere(Collider* colliderA, Collider* colliderB);
};
