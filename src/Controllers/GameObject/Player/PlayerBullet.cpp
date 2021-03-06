#include "PlayerBullet.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標 </param>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	velocity_ = velocity;

	//テクスチャ読み込み
	texturehandle_ = TextureManager::Load("images/black.png");

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//ワールド変換の初期化
	worldTransform_.Initialize();
}

/// <summary>
/// 更新
/// </summary>
void PlayerBullet::Update() {

	//座標を移動させる(1フレーム分の移動を差し込む)
	worldTransform_.translation_ += velocity_;

	//ワールドトランスフォームの更新
	MyMath::AffineTransformation(worldTransform_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

/// <summary>
/// 描画
/// </summary>
/// /// <param name="viewProjection">ビュープロジェクション</param>
void PlayerBullet::Draw(const ViewProjection viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

//衝突を検出したら呼び出されるコールバック関数
void PlayerBullet::OnCollision() {
	//デスフラグを立てる
	isDead_ = true;
}

//ワールド座標を取得
Vector3 PlayerBullet::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

float PlayerBullet::GetRadius() { return radius_; }
