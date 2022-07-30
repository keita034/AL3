#pragma once
#include <cassert>
#include <list>
#include <memory>

#include "Collider.h"
#include "MyMath.h"
#include "PlayerBullet.h"

class CollisionManager {
  private:
	//コライダーリスト
	std::list<Collider*> colliders_;

  public:
	/// <summary>
	/// コライダーをリストに登録
	/// </summary>
	void ColliderSet(Collider* collider);

	/// <summary>
	/// コライダーリストをクリア
	/// </summary>
	void ListClear();

	/// <summary>
	/// 衝突判定と応答
	/// </summary
	void CheckCollisions();

  private:
	/// <summary>
	/// 球と球の衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckSphereToSphere(Collider* colliderA, Collider* colliderB);
};
