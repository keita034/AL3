#pragma once
#include "DebugText.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "input.h"

#include "EnemyBullet.h"
#include "MyMath.h"
#include "Player.h"

class Player;

//行動フェーズ
enum class Phase {
	Approach, //接近する
	Leave,    //離脱する

};

/// <summary>
///敵
/// </summary>
class Enemy {
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

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
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	Vector3 GetWorldPosition();

	//発射間隔
	static const int kFireInterval = 60;

  private:
	/// <summary>
	/// 接近フェーズ移動処理
	/// </summary>
	void ApproachVelocity();

	/// <summary>
	/// 離脱フェーズ移動処理
	/// </summary>
	void LeaveVelocity();

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t texturehandle_ = 0u;

	//キーボード操作
	Input* input_ = nullptr;

	//デバッグ用表示
	DebugText* debugText_ = nullptr;

	//速度
	Vector3 approachVelocity_ = {0, 0, -0.3f};
	Vector3 leaveVelocity_ = {-0.1f, 0.1f, -0.1f};

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t fileTimer_ = 0;

	//自キャラ
	Player* player_ = nullptr;
};
