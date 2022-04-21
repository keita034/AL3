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

	random_device seedGen;

	mt19937_64 engine(seedGen());

	uniform_real_distribution<float> rotDist(0.0f, XM_2PI);

	uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	model_ = Model::Create();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	worldTransform_[0].translation_ = {0.0f, 5.0f, 0.0f};

	worldTransform_[1].translation_ = {-4.33f, -2.5f, 0.0f};

	worldTransform_[2].translation_ = {4.33f, -2.5f, 0.0f};

	for (int i = 0; i < _countof(worldTransform_); i++) {
		//ワールドトランム初期化
		worldTransform_[i].Initialize();
	}

	viewProjection_.eye = {0.0f, 0.0f, -25.0f};

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//カメラ切り替え
	if (input_->TriggerKey(DIK_SPACE) && !MoveFlag) {
		MoveFlag = true;
	}

 	if (MoveFlag) {
		switch (CameraNum) {
		case 0:
			if (viewProjection_.target.y < worldTransform_[0].translation_.y) {
				viewProjection_.target.y += (worldTransform_[0].translation_.y + -worldTransform_[2].translation_.y) / 100;
			} else {
				viewProjection_.target.y = worldTransform_[0].translation_.y;
			}

			if (viewProjection_.target.x > worldTransform_[0].translation_.x) {
				viewProjection_.target.x -= worldTransform_[2].translation_.x / 100;
			} else {
				viewProjection_.target.x = worldTransform_[0].translation_.x;
			}

			if (viewProjection_.target.x == worldTransform_[0].translation_.x && viewProjection_.target.y == worldTransform_[0].translation_.y) {
				MoveFlag = false;
				CameraNum++;
			}
			break;
		case 1:
			if (viewProjection_.target.y > worldTransform_[1].translation_.y) {
				viewProjection_.target.y += (-worldTransform_[0].translation_.y + worldTransform_[1].translation_.y) / 100;
			}
			if (viewProjection_.target.x > worldTransform_[1].translation_.x) {
				viewProjection_.target.x += worldTransform_[1].translation_.x / 100;
			} else {
				viewProjection_.target.x = worldTransform_[1].translation_.x;
			}

			if (viewProjection_.target.x == worldTransform_[1].translation_.x && viewProjection_.target.y == worldTransform_[1].translation_.y) {
				MoveFlag = false;
				CameraNum++;
			}

			break;
		case 2:
			if (viewProjection_.target.x < worldTransform_[2].translation_.x) {
				viewProjection_.target.x += (worldTransform_[2].translation_.x + -worldTransform_[1].translation_.x) / 100;
			} else {
				viewProjection_.target.x = worldTransform_[2].translation_.x;
			}

			if (viewProjection_.target.x == worldTransform_[2].translation_.x) {
				MoveFlag = false;
				CameraNum = 0;
			}
			break;
		}
	}
	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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