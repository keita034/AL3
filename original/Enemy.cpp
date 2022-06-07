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
}

void Enemy::Update() {

	worldTransform_.translation_ += velocity_;
	//ワールド行列計算
	worldTransform_.AffineTransformation();

	//デバッグ用表示
	debugText_->SetPos(0, 60);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

/// <summary>
/// 描画
/// </summary>
/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}