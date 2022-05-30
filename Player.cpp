#include "Player.h"


#include"ViewProjection.h"
#include"Affine.h"
#include<cassert>

void Player::Move() {
	//キャラクター移動処理
	{
		//キャラクター移動処理
		Vector3 move = {0, 0, 0};
		//キャラクター移動ベクトル
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		worldTransform_.translation_ += move;
	}

	{
		//キャラクター移動処理
		Vector3 move = {0, 0, 0};
		//キャラクター移動ベクトル
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_UP)) {
			move = {0, kCharacterSpeed, 0};
		} else if (input_->PushKey(DIK_DOWN)) {
			move = {0, -kCharacterSpeed, 0};
		}

		worldTransform_.translation_ += move;

	}

	//移動制限
	{
		const float kMoveLimitX = 35.0f;
		const float kMoveLimitY = 19.0f;

		worldTransform_.translation_.x = max(worldTransform_.translation_.x,-kMoveLimitX);
		worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	}
	
	AffineTransformation(worldTransform_);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NUULポインタ」チェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	texturehandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {
	Move();

	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

/// <summary>
/// 描画
/// </summary>
/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

