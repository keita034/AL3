#include "EnemyBullet.h"
#include"Player.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標 </param>
void EnemyBullet::Initialize(std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity) {

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	velocity_ = velocity;

	//テクスチャ読み込み
	texturehandle_ = TextureManager::Load("images/red.png");

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//Z方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	//ワールド変換の初期化
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	Vector3 temp = velocity;
	temp.y = 0.0f;
	worldTransform_.rotation_.x = std::atan2(-velocity.y, MyMath::Vector3Length(temp));
	//ワールド変換の初期化
	worldTransform_.Initialize();
}

// 更新
void EnemyBullet::Update() {
	//敵弾から自キャラへのベクトルを計算
	Vector3 toPlayer =
		MyMath::Vector3Sub(player_->GetWorldPosition(), MyMath::GetWorldPosition(worldTransform_));

	//ベクトルを正規化する
	MyMath::Vector3Normalize(toPlayer);
	MyMath::Vector3Normalize(velocity_);
	//球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = MyMath::Vector3Mul(MyMath::Slerp(velocity_, toPlayer, 0.01f), 1.0f);

	//座標を移動させる(1フレーム分の移動を差し込む)
	worldTransform_.translation_ += velocity_;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 temp = velocity_;
	temp.y = 0.0f;
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, MyMath::Vector3Length(temp));

	//ワールドトランスフォームの更新
	MyMath::AffineTransformation(worldTransform_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

// 描画
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

//衝突を検出したら呼び出されるコールバック関数
void EnemyBullet::OnCollision() {
	//デスフラグを立てる
	isDead_ = true;
}

//ワールド座標を取得
Vector3 EnemyBullet::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

float EnemyBullet::GetRadius() { return radius_; }