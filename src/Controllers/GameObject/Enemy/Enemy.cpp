#include "Enemy.h"
#include "EnemyStateApproach.h"
#include "GameScene.h"
#include <cassert>

Enemy::~Enemy() {}

// 初期化
void Enemy::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, const Vector3& position) {
	// NUULポインタ」チェック

	//引数として受け取ったデータをメンバ変数に記録する
	texturehandle_ = textureHandle;

	model_ = model;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	state_ = std::make_unique<EnemyStateApproach>();

	//接近フェーズ初期化
	approachPhaseInt();
}

// 更新処理
void Enemy::Update() {

	//移動処理
	//削除
	timedCalls_.remove_if([](std::unique_ptr<TimedCall>& call) { return call->IsFinished(); });

	//移動処理
	state_->Update(this);

	//ワールド行列計算
	MyMath::AffineTransformation(worldTransform_);
}

// 描画
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
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
	newBullet->Initialize(model_,worldTransform_.translation_, velocity);
	newBullet->SetPlayer(player_);
	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

//接近フェーズ初期化
void Enemy::approachPhaseInt() {

	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind(&Enemy::FireReset, this), kFireInterval));
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

void Enemy::OnCollision() { isDead_ = true; }

float Enemy::GetRadius() { return radius_; }

void Enemy::FireReset() {
	Fire();
	//発射タイムをリセットする
	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind(&Enemy::FireReset, this), kFireInterval));
}