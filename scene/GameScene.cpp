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
	float radian = angle *  PI /180;
	return radian;
}

float ChangeDira(const float& angle)
{
	float radian = angle * 180 / PI ;
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

	//カメラ視点座標を設定
	// viewProjection_.eye = {0,0,-10};

	//カメラ注目点座標を設定
	// viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定（右上45ど指定）
	// viewProjection_.up = {cosf(PI/4.0f),cosf(PI / 4.0f) ,0.0f};

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = ChangeRadi(10.0f);

	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;

	//ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

	//乱数シード生成器
	random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	uniform_real_distribution<float> dist(-10, 10);
	uniform_real_distribution<float> angle(0, PI2);

	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();
		//スケーリングを設定
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		//回転角を設定
		worldTransform.rotation_ = {angle(engine), angle(engine), angle(engine)};
		//平行移動を設定
		worldTransform.translation_ = {dist(engine), dist(engine), dist(engine)};

		AffineTransformation(worldTransform);
	}
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

	////注視点移動処理
	//{
	//	//注視点の移動移動ベクトル
	//	Vector3 move = {0, 0, 0};

	//	//注視点の移動の速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move = {kTargetSpeed, 0, 0};
	//	} else if (input_->PushKey(DIK_RIGHT)) {
	//		move = {-kTargetSpeed, 0, 0};
	//	}

	//	//注視点移動（ベクトルの加算）
	//	viewProjection_.target += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
		  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		  viewProjection_.target.z);
	//}

	////視点移動処理
	//{
	//	//視点の移動移動ベクトル
	//	Vector3 move = {0, 0, 0};

	//	//視点の移動の速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move = {0, 0, +kEyeSpeed};
	//	} else if (input_->PushKey(DIK_S)) {
	//		move = {0, 0, -kEyeSpeed};
	//	}

	//	//視点移動（ベクトルの加算）
	//	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
		  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}

	////上方向回転処理
	//{
	//	//上方向の回転の速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;

	//		// 2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, PI2);
	//	}

	//	//上方向ベクトルを計算（半径1の円周上の座標）
	//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
		  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	//}

	// Fov変更処理
	{
		//上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
		}

		//下キーで視野角が狭まる
		if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf(
			"fovAngleY:%f", ChangeDira(viewProjection_.fovAngleY));
	}

	// クリップ距離変更処理
	{
		//上下キーでニアクリップ距離増減
		if (input_->PushKey(DIK_W))
		{
			viewProjection_.nearZ += 0.1f;
		}

		if (input_->PushKey(DIK_S))
		{
			viewProjection_.nearZ -= 0.1f;
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf(
			"nearZ:%f", viewProjection_.nearZ);
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
	for (WorldTransform& worldTransform : worldTransforms_) {
		// 3dモデル描画
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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
