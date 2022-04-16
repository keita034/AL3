﻿#include "GameScene.h"
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

	//ワールドトランム初期化
	worldTransform_.Initialize();

	//ビュープロジェクション初期化
	for (size_t i = 0; i < _countof(viewProjection_); i++) {

		viewProjection_[i].eye = {posDist(engine), posDist(engine), posDist(engine)};

		viewProjection_[i].Initialize();
	}
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		CameraNum++;

		if (CameraNum >= 3){
			CameraNum = 0;
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

	model_->Draw(worldTransform_, viewProjection_[CameraNum], textureHandle_);

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