#pragma once
#include "DebugText.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "input.h"

#include "BaseEnemyState.h"
#include "Collider.h"
#include "EnemyBullet.h"
#include "MyMath.h"
#include "TimedCall.h"

class Player;
// GameSceneの前方宣言
class GameScene;

//行動フェーズ
enum class Phase {
	Approach, //接近する
	Leave,    //離脱する

};

/// <summary>
///敵
/// </summary>
class Enemy : public Collider {
  public:
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void approachPhaseInt();

	/// <summary>
	/// プレイヤーのアドレスをセット
	/// </summary>
	/// <param name="player">プレイヤーのアドレス</param>
	void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }

	/// <summary>
	/// 半径を所得
	/// </summary>
	float GetRadius();

	/// <summary>
	/// ゲームシーンのアドレスをセット
	/// </summary>
	/// <param name="gameScene">ゲームシーンのアドレス</param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	///デスフラグゲッター
	bool IsDead() const;

	//発射間隔
	static const int kFireInterval = 60;

	void ChangeState(BaseEnemyState* baseEnemyState);

	WorldTransform GetWorldTransform();

	void MoveTranslation(Vector3& move);

	Vector3 GetApproachVelocity();

	Vector3 GetLeaveVelocity();

	std::list<std::unique_ptr<TimedCall>>& GetTimedCalls();

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireReset();

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

	//キーボード操作
	Input* input_ = nullptr;

	//デバッグ用表示
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 approachVelocity_ = {0, 0, -0.3f};
	Vector3 leaveVelocity_ = {-0.1f, 0.1f, -0.1f};

	//自キャラ
	std::shared_ptr<Player> player_;

	//半径
	const float radius_ = 1.0f;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	std::unique_ptr<BaseEnemyState> state_;

	//時限発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_;
};

inline bool Enemy::IsDead() const { return isDead_; }

inline void Enemy::ChangeState(BaseEnemyState* baseEnemyState) { state_.reset(baseEnemyState); }

inline WorldTransform Enemy::GetWorldTransform() { return worldTransform_; }

inline void Enemy::MoveTranslation(Vector3& move) { worldTransform_.translation_ += move; }

inline Vector3 Enemy::GetApproachVelocity() { return approachVelocity_; }

inline Vector3 Enemy::GetLeaveVelocity() { return leaveVelocity_; }

inline std::list<std::unique_ptr<TimedCall>>& Enemy::GetTimedCalls() { return timedCalls_; }