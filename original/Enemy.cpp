#include "Enemy.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NUULポインタ」チェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	texturehandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	//接近フェーズ初期化
	approachPhaseInt();
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	//移動処理
	switch (phase_) {
	case Phase::Approach: //接近フェーズ
		ApproachVelocity();
		break;
	case Phase::Leave: //離脱フェーズ
		LeaveVelocity();
		break;
	}
	//ワールド行列計算
	worldTransform_.AffineTransformation();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デバッグ用表示
	debugText_->SetPos(0, 60);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(0, 80);
	if (phase_ == Phase::Approach) {
		debugText_->Printf("Phase:Approach");
	} else {
		debugText_->Printf("Phase:Leave");
	}
}

/// <summary>
/// 描画
/// </summary>
/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

/// <summary>
/// 弾発射
/// </summary>
void Enemy::Fire() {
	//自キャラの座標をコピー
	Vector3 position = worldTransform_.translation_;

	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = worldTransform_.VecMatMul(velocity);

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, position, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

//接近フェーズ初期化
void Enemy::approachPhaseInt()
{
	//発射タイマーを初期化
	fileTimer_ = kFireInterval;
}

// 接近フェーズ移動処理
void Enemy::ApproachVelocity() {

	//発射タイマーカウントダウン
	fileTimer_--;
	//指定時間に達した
	if (fileTimer_==0)
	{
		//弾の発射
		Fire();
		//発射タイマーを初期化
		fileTimer_ = kFireInterval;
	}

	//移動(ベクトルを加算)
	worldTransform_.translation_ += approachVelocity_;
	//既定の位置に到着したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

//離脱フェーズ移動処理
void Enemy::LeaveVelocity() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ += leaveVelocity_;
}
