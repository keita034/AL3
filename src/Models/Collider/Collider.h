#pragma once
#include "Vector3.h"
#include <cstdint>
#include"CollisionConfig.h"

/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider {
  private:
	//�Փ˔��a
	float radius_ = 1.0f;
	//�Փˑ���(����)
	uint32_t collisionAttribute_ = 0xffffffff;
	//�Փ˃}�X�N(����)
	uint32_t CollisionMask_ = 0xffffffff;

  public:
	//���a���擾
	float GetRadius();
	//���a����ݒ�
	void SetRadius(float radius);
	//�Փˎ��ɌĂ΂��֐�
	virtual void OnCollision();
	//���[���h���W���擾
	virtual Vector3 GetWorldPosition() = 0;
	//�Փˑ���(����)���擾
	uint32_t GetCollisionAttribute();
	//�Փˑ���(����)���擾
	void SetCollisionAttribute(uint32_t collisionAttribute);
	//�Փˑ���(����)���擾
	uint32_t GetCollisionMask();
	//�Փˑ���(����)��ݒ�
	void SetCollisionMask(uint32_t CollisionMask);
};

inline float Collider::GetRadius() { return radius_; }

inline void Collider::SetRadius(float radius) { radius_ = radius; }

inline uint32_t Collider::GetCollisionAttribute() { return collisionAttribute_; }

inline void Collider::SetCollisionAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;
}

inline uint32_t Collider::GetCollisionMask() { return CollisionMask_; }

inline void Collider::SetCollisionMask(uint32_t CollisionMask) { CollisionMask_ = CollisionMask; }
