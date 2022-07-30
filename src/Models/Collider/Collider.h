#pragma once
#include "Vector3.h"
#include <cstdint>
#include"CollisionConfig.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
  private:
	//衝突半径
	float radius_ = 1.0f;
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;

  public:
	//半径を取得
	float GetRadius();
	//半径をを設定
	void SetRadius(float radius);
	//衝突時に呼ばれる関数
	virtual void OnCollision();
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
	//衝突属性(自分)を取得
	uint32_t GetCollisionAttribute();
	//衝突属性(自分)を取得
	void SetCollisionAttribute(uint32_t collisionAttribute);
	//衝突属性(相手)を取得
	uint32_t GetCollisionMask();
	//衝突属性(相手)を設定
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
