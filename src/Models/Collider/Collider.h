#pragma once
#include "Vector3.h"

/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider {
  private:
	//�Փ˔��a
	float radius_ = 1.0f;

  public:
	//���a���擾
	float GetRadius();
	//���a����ݒ�
	void SetRadius(float radius);
	//�Փˎ��ɌĂ΂��֐�
	virtual void OnCollision();
	//���[���h���W���擾
	virtual Vector3 GetWorldPosition() = 0;
};

inline float Collider::GetRadius() { return radius_; }

inline void Collider::SetRadius(float radius) { radius_ = radius; }


