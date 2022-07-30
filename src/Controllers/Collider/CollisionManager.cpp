#include "CollisionManager.h"

void CollisionManager::ColliderSet(Collider* collider) {
	assert(collider);
	colliders_.push_back(collider);
}

void CollisionManager::ListClear() {
	if (!colliders_.empty()) {
		colliders_.clear();
	}
}

void CollisionManager::CheckCollisions() {
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//�C�e���[�^B�C�e���[�^A�̎��̗v�f�����(�d����������)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//�y�A�̓����蔻��
			CheckSphereToSphere(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckSphereToSphere(Collider* colliderA, Collider* colliderB) {
	//�Փ˃t�B���^�����O
	if (
	  colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask() ||
	  colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) {

	} else {
		return;
	}

	//����Ώ�A��B�̍��W
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	//���WA��B�̋��������߂�
	float distance = MyMath::Vector3Length(MyMath::Vector3Sub(posA, posB));

	//���e�ƓG�e�̔��a
	float radius = colliderA->GetRadius() + colliderB->GetRadius();

	//���Ƌ��̌�������
	if (distance <= radius) {
		//�R���C�_�[A�̏Փˎ��R�[���o�b�N���Ăяo��
		colliderA->OnCollision();
		//�R���C�_�[B�̏Փˎ��R�[���o�b�N���Ăяo��
		colliderB->OnCollision();
	}
}
