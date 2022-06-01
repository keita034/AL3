#include "Player.h"

#include "Affine.h"
#include "ViewProjection.h"

#include<DirectXMath.h>
#include <cassert>

/// <summary>
/// 移動
/// </summary>
void Player::Move() {
	//キャラクター移動処理
	{
		//キャラクター移動ベクトル
		Vector3 move = {0, 0, 0};

		const float kCharacterSpeed = 0.5f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharacterSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharacterSpeed, 0, 0};
		}

		worldTransform_.translation_ += move;
	}

	{
		//キャラクター移動ベクトル
		Vector3 move = {0, 0, 0};

		const float kCharacterSpeed = 0.5f;

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

		worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
	}
}

/// <summary>
/// 旋回
/// </summary>
void Player::Rotate() {

	//回転処理
	{
		//回転の速さ
		const float kChestaRotSpeed = 0.07f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_F)) {
			worldTransform_.rotation_.y -= kChestaRotSpeed;
		} else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += kChestaRotSpeed;
		}
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//自キャラの座標をコピー
		Vector3 position = worldTransform_.translation_;

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0,0,kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = VecMatMul(velocity,worldTransform_.matWorld_);

		// 弾を生成し、初期化
		std::unique_ptr< PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//移動
	Move();

	//旋回
	Rotate();

	//ワールド行列計算
	AffineTransformation(worldTransform_);

	//攻撃
	Attack();

	//弾更新
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デバッグ用表示
	debugText_->SetPos(0, 0);
	debugText_->Printf(
	  "Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf(
		"Rot:%f", worldTransform_.rotation_.y);
}

/// <summary>
/// 描画
/// </summary>
/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}
