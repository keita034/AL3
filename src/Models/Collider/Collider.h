#pragma once
#include "Vector3.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
  private:
	//衝突半径
	float radius_ = 1.0f;

  public:
	//半径を取得
	float GetRadius();
	//半径をを設定
	void SetRadius(float radius);
	//衝突時に呼ばれる関数
	virtual void OnCollision();
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
};

inline float Collider::GetRadius() { return radius_; }

inline void Collider::SetRadius(float radius) { radius_ = radius; }


