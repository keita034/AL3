#pragma once
#include "DebugText.h"
#include "Model.h"
#include "MyMath.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <cassert>

#include "Collider.h"

class Player;

/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet : public Collider {

  public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name = "model">モデル</param>
	/// <param name = "position">初期座標</param>
	void Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name = "viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	///デスフラグゲッター
	bool IsDead() const;

	/// <summary>
	/// 半径を所得
	/// </summary>
	float GetRadius();

	void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }

	//衝突時に呼ばれる関数
	void OnCollision() override;

	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	Vector3 GetWorldPosition() override;

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	std::shared_ptr<Model> model_;

	//テクスチャハンドル
	uint32_t texturehandle_ = 0u;

	//デバッグ用表示
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	std::shared_ptr<Player> player_;
};

inline bool EnemyBullet::IsDead() const { return isDead_; }
