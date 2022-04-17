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

	//ワールドトランム初期化
	worldTransform_.Initialize();

	//ビュープロジェクション初期化
		viewProjection_.Initialize();
}

void GameScene::Update() {

	// 度数法を変換
	float rad = Angle * XM_PI / 180.0f;
	
	//円の位置を割り出す
	float add_x = cos(rad) * 10.0f;
	float add_z = sin(rad) * 10.0f;
	
	//中心座標に位置を加算
 	viewProjection_.eye.x = worldTransform_.translation_.x + add_x;
	viewProjection_.eye.z = worldTransform_.translation_.z + add_z;

	//角度加算
	Angle += 1.0f;

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

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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