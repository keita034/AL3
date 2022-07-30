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
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckSphereToSphere(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckSphereToSphere(Collider* colliderA, Collider* colliderB) {
	//衝突フィルタリング
	if (
	  colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask() ||
	  colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) {

	} else {
		return;
	}

	//判定対象AとBの座標
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	//座標AとBの距離を求める
	float distance = MyMath::Vector3Length(MyMath::Vector3Sub(posA, posB));

	//自弾と敵弾の半径
	float radius = colliderA->GetRadius() + colliderB->GetRadius();

	//球と球の交差判定
	if (distance <= radius) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}
