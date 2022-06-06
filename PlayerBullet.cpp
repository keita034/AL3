#include "PlayerBullet.h"

#include <cassert>

#include "ViewProjection.h"
#include"TextureManager.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標 </param>
void PlayerBullet::Initialize(Model* model, const Vector3 position) {

	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//テクスチャ読み込み
	texturehandle_ = TextureManager::Load("black.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

/// <summary>
/// 更新
/// </summary>
void PlayerBullet::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.AffineTransformation();
}

/// <summary>
/// 描画
/// </summary>
/// /// <param name="viewProjection">ビュープロジェクション</param>
void PlayerBullet::Draw(const ViewProjection viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}
