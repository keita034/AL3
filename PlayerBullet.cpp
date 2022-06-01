#include "PlayerBullet.h"

#include <cassert>

#include "ViewProjection.h"
#include"TextureManager.h"

#include "Affine.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="position">初期座標 </param>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	velocity_ = velocity;

	//テクスチャ読み込み
	texturehandle_ = TextureManager::Load("black.png");

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
	AffineTransformation(worldTransform_);

	if (--deathTimer_ <= 0)
	{
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
