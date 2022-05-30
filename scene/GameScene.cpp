#include "GameScene.h"
#include "Affine.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

//円周率
const float PI = 3.1415926535f;

const float PI2 = PI * 2;

using namespace std;

float ChangeRadi(const float& angle) {
	float radian = angle * PI / 180;
	return radian;
}

float ChangeDira(const float& angle) {
	float radian = angle * 180 / PI;
	return radian;
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデル生成
	model_ = Model::Create();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

	//キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();
	//脊椎
	worldTransforms_[PartId::kSpine].translation_ = {0, 4.5f, 0};
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].Initialize();

	//上半身
	//胸
	worldTransforms_[PartId::kChest].translation_ = {0, 1.3f, 0};
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].Initialize();

	//頭
	worldTransforms_[PartId::kHead].translation_ = {0, 3.0f, 0};
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].Initialize();

	//左腕
	worldTransforms_[PartId::kArmL].translation_ = {3.0f, 0, 0};
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].Initialize();

	//右腕
	worldTransforms_[PartId::kArmR].translation_ = {-3.0f, 0, 0};
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].Initialize();

	//下半身
	// 尻
	worldTransforms_[PartId::kHip].translation_ = {0, -2.0, 0};
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].Initialize();

	//左足
	worldTransforms_[PartId::kLegL].translation_ = {3.0f, -3.0, 0};
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].Initialize();

	//右足
	worldTransforms_[PartId::kLegR].translation_ = {-3.0f, -3.0, 0};
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].Initialize();
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

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

		worldTransforms_[PartId::kRoot].translation_ += move;

		AffineTransformation(worldTransforms_[PartId::kRoot]);

		debugText_->Printf(
		  "Root:(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x,
		  worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[0].translation_.z);
	}

	//上半身回転処理
	{
		//上半身の回転の速さ
		const float kChestaRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U))
		{
			worldTransforms_[PartId::kChest].rotation_.y -= kChestaRotSpeed;
		}
		else if (input_->PushKey(DIK_I))
		{
			worldTransforms_[PartId::kChest].rotation_.y += kChestaRotSpeed;
		}
	}

	//下半身回転処理
	{
		//下半身の回転の速さ
		const float kHipRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J))
		{
			worldTransforms_[PartId::kHip].rotation_.y -= kHipRotSpeed;
		}
		else if (input_->PushKey(DIK_K))
		{
			worldTransforms_[PartId::kHip].rotation_.y += kHipRotSpeed;
		}

	}

	//子の更新
	{
		for (int i = 1; i < PartId::kNumPartId; i++) {
			worldTransforms_[i].ParenChildUpdate();
		}
	}


}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		if (i<2)
		{
			continue;
		}

		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
