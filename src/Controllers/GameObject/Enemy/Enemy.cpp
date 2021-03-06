#include "Enemy.h"
#include <cassert>

// 初期化
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

// 更新処理
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
	MyMath::AffineTransformation(worldTransform_);

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

// 描画
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

// 弾発射
void Enemy::Fire() {

	assert(player_);

	//弾の速度
	const float kBulletSpeed = 1.0f;

	//プレイヤーのワールド座標を取得
	Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	Vector3 enemyPos = GetWorldPosition();
	//敵キャラ->自キャラの差分ベクトルを求める
	Vector3 velocity = MyMath::Vector3Sub(playerPos, enemyPos);
	//ベクトルの正規化
	velocity = MyMath::Vector3Normalize(velocity);
	//ベクトルの長さを、早さに合わせる
	velocity *= kBulletSpeed;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

//接近フェーズ初期化
void Enemy::approachPhaseInt() {
	//発射タイマーを初期化
	fileTimer_ = kFireInterval;
}

// ワールド座標を所得
Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {}

float Enemy::GetRadius() { return radius_; }

// 接近フェーズ移動処理
void Enemy::ApproachVelocity() {

	//発射タイマーカウントダウン
	fileTimer_--;
	//指定時間に達した
	if (fileTimer_ == 0) {
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
