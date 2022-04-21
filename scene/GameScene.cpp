#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		for (size_t j = 0; j < _countof(worldTransform_); j++) {
			worldTransform_[i][j].scale_ = {1.0f, 1.0f, 1.0f};

			//平行移動を設定
			worldTransform_[i][j].translation_ = {-16.0f + j * 4.0f, -16.0f + i * 4.0f, 0};

			//ワールドトランスフォーム初期化
			worldTransform_[i][j].Initialize();
		}
	}

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

#pragma region 連続移動処理

#pragma endregion

	// Fov変更処理
	//上キーで視野角が広がる
	if (input_->PushKey(DIK_UP)) {
		viewProjection_.fovAngleY += 0.01f;
		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
	}
	//下キーで視野角が狭まる
	if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.fovAngleY -= 0.01f;
		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	}

	//視点の移動ベクトル
	XMFLOAT3 Eyemove = {0, 0, 0};

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		Eyemove = {0, kEyeSpeed, 0};

	} else if (input_->PushKey(DIK_S)) {
		Eyemove = {0, -kEyeSpeed, 0};
	}

	if (input_->PushKey(DIK_D)) {
		Eyemove = {kEyeSpeed, 0, 0};

	} else if (input_->PushKey(DIK_A)) {
		Eyemove = {-kEyeSpeed, 0, 0};
	}

	//視点移動（ベクトルの加算）
	viewProjection_.target.x += Eyemove.x;
	viewProjection_.target.y += Eyemove.y;
	viewProjection_.target.z += Eyemove.z;

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));

	//デバッグ用表示
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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
	model_ = Model::Create();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (size_t i = 0; i < _countof(worldTransform_); i++) {
		for (size_t j = 0; j < _countof(worldTransform_); j++) {
			model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
		}
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
