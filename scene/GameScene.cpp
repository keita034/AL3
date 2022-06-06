#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

//円周率
const float PI = 3.1415926535f;

//単位行列
void IdentityMatrix(Matrix4& matrix) {
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
}

#pragma region アフィン変換分解

//スケーリング行列生成
Matrix4 ScalingForm(float x, float y, float z) {
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = x;
	matScale.m[1][1] = y;
	matScale.m[2][2] = z;
	matScale.m[3][3] = 1.0f;

	return matScale;
}

// Z軸回転行列を生成
Matrix4 RotationZForm(float angle) {
	// Z軸回転行列を宣言
	Matrix4 matRotZ;

	// Z軸回転行列の各要素を設定する
	matRotZ.m[0][0] = cos(angle);
	matRotZ.m[0][1] = sin(angle);

	matRotZ.m[1][0] = -sin(angle);
	matRotZ.m[1][1] = cos(angle);

	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;

	return matRotZ;
}

// X軸回転行列を生成
Matrix4 RotationXForm(float angle) {
	// X軸回転行列を宣言
	Matrix4 matRotX;

	// X軸回転行列の各要素を設定する
	matRotX.m[1][1] = cos(angle);
	matRotX.m[1][2] = sin(angle);

	matRotX.m[2][1] = -sin(angle);
	matRotX.m[2][2] = cos(angle);

	matRotX.m[0][0] = 1.0f;
	;
	matRotX.m[3][3] = 1.0f;

	return matRotX;
}

// Y軸回転行列を生成
Matrix4 RotationYForm(float angle) {
	// Y軸回転行列を宣言
	Matrix4 matRotY;

	// Y軸回転行列の各要素を設定する
	matRotY.m[0][0] = cos(angle);
	matRotY.m[0][2] = -sin(angle);

	matRotY.m[2][0] = sin(angle);
	matRotY.m[2][2] = cos(angle);

	matRotY.m[1][1] = 1.0f;
	matRotY.m[3][3] = 1.0f;

	return matRotY;
}

//平行移動行列を生成
Matrix4 TransferForm(float x, float y, float z) {
	//平行移動行列を宣言
	Matrix4 matTrans;
	//単位行列を代入
	IdentityMatrix(matTrans);

	//移動量を行列に設定する
	matTrans.m[3][0] = x;
	matTrans.m[3][1] = y;
	matTrans.m[3][2] = z;

	return matTrans;
}

//ワールド行列を生成
Matrix4 WorldForm(
  Matrix4& scale, Matrix4& rotx, Matrix4& roty, Matrix4& rotz, Matrix4& trans) {
	Matrix4 matWorld;

	//単位行列を代入
	IdentityMatrix(matWorld);

	// matWorld_にスケーリング倍率を掛け算
	matWorld *= scale;

	// matWorld_にZ軸回転行列を掛け算
	if (rotz.m[0][0] != 0) {
		matWorld *= rotz;
	}

	// matWorld_にX軸回転行列を掛け算
	if (rotx.m[2][2] != 0) {
		matWorld *= rotx;
	}

	// matWorld_にY軸回転行列を掛け算
	if (roty.m[0][0] != 0) {
		matWorld *= roty;
	}

	// matWorld_に移動量を掛け算
	matWorld *= trans;

	return matWorld;
}
#pragma endregion

#pragma region アフィン変換

//アフィン変換自分で
void AffineTransformation(WorldTransform& worldtransform) {

	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldtransform.scale_.x;
	matScale.m[1][1] = worldtransform.scale_.y;
	matScale.m[2][2] = worldtransform.scale_.z;
	matScale.m[3][3] = 1.0f;

	// Z軸回転行列を宣言
	Matrix4 matRotZ;
	// Z軸回転行列の各要素を設定する
	if (worldtransform.rotation_.z != 0) {
		matRotZ.m[0][0] = cos(worldtransform.rotation_.z);
		matRotZ.m[0][1] = sin(worldtransform.rotation_.z);

		matRotZ.m[1][0] = -sin(worldtransform.rotation_.z);
		matRotZ.m[1][1] = cos(worldtransform.rotation_.z);

		matRotZ.m[2][2] = 1.0f;
		matRotZ.m[3][3] = 1.0f;
	}

	// X軸回転行列を宣言
	Matrix4 matRotX;
	// X軸回転行列の各要素を設定する
	if (worldtransform.rotation_.x != 0) {
		matRotX.m[1][1] = cos(worldtransform.rotation_.x);
		matRotX.m[1][2] = sin(worldtransform.rotation_.x);

		matRotX.m[2][1] = -sin(worldtransform.rotation_.x);
		matRotX.m[2][2] = cos(worldtransform.rotation_.x);

		matRotX.m[0][0] = 1.0f;
		matRotX.m[3][3] = 1.0f;
	}

	// Y軸回転行列を宣言
	Matrix4 matRotY;

	// Y軸回転行列の各要素を設定する
	if (worldtransform.rotation_.y != 0) {
		matRotY.m[0][0] = cos(worldtransform.rotation_.y);
		matRotY.m[0][2] = -sin(worldtransform.rotation_.y);

		matRotY.m[2][0] = sin(worldtransform.rotation_.y);
		matRotY.m[2][2] = cos(worldtransform.rotation_.y);

		matRotY.m[1][1] = 1.0f;
		matRotY.m[3][3] = 1.0f;
	}

	//平行移動行列を宣言
	Matrix4 matTrans;
	//単位行列を代入
	IdentityMatrix(matTrans);

	//移動量を行列に設定する
	matTrans.m[3][0] = worldtransform.translation_.x;
	matTrans.m[3][1] = worldtransform.translation_.y;
	matTrans.m[3][2] = worldtransform.translation_.z;

	//単位行列を代入
	IdentityMatrix(worldtransform.matWorld_);

	// matWorld_にスケーリング倍率を掛け算
	worldtransform.matWorld_ *= matScale;

	// matWorld_にZ軸回転行列を掛け算
	if (worldtransform.rotation_.z != 0.0f) {
		worldtransform.matWorld_ *= matRotZ;
	}
	// matWorld_にX軸回転行列を掛け算
	if (worldtransform.rotation_.x != 0.0f) {
		worldtransform.matWorld_ *= matRotX;
	}
	// matWorld_にY軸回転行列を掛け算
	if (worldtransform.rotation_.y != 0.0f) {
		worldtransform.matWorld_ *= matRotY;
	}

	// matWorld_に移動量を掛け算
	worldtransform.matWorld_ *= matTrans;

	//行列の転送
	worldtransform.TransferMatrix();
}

//アフィン変換関数
void AffineTransformationFunction(WorldTransform& worldtransform) {

	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定する
	matScale = MathUtility::Matrix4Scaling(
	  worldtransform.scale_.x, worldtransform.scale_.y, worldtransform.scale_.z);

	// Z軸回転行列を宣言
	Matrix4 matRotZ;
	// Z軸回転行列の各要素を設定する
	if (worldtransform.rotation_.z != 0) {
		matRotZ = MathUtility::Matrix4RotationZ(worldtransform.rotation_.z);
	}

	// X軸回転行列を宣言
	Matrix4 matRotX;
	// X軸回転行列の各要素を設定する
	if (worldtransform.rotation_.x != 0) {
		matRotX = MathUtility::Matrix4RotationX(worldtransform.rotation_.x);
	}
	// Y軸回転行列を宣言
	Matrix4 matRotY;
	// Y軸回転行列の各要素を設定する
	if (worldtransform.rotation_.y != 0) {
		matRotY = MathUtility::Matrix4RotationY(worldtransform.rotation_.y);
	}

	//平行移動行列を宣言・単位行列を代入
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//移動量を行列に設定する
	matTrans = MathUtility::Matrix4Translation(
	  worldtransform.translation_.x, worldtransform.translation_.y, worldtransform.translation_.z);

	//単位行列を代入
	worldtransform.matWorld_ = MathUtility::Matrix4Identity();

	// matWorld_にスケーリング倍率を掛け算
	worldtransform.matWorld_ *= matScale;

	// matWorld_にZ軸回転行列を掛け算
	if (worldtransform.rotation_.z != 0.0f) {
		worldtransform.matWorld_ *= matRotZ;
	}
	// matWorld_にX軸回転行列を掛け算
	if (worldtransform.rotation_.x != 0.0f) {
		worldtransform.matWorld_ *= matRotX;
	}
	// matWorld_にY軸回転行列を掛け算
	if (worldtransform.rotation_.y != 0.0f) {
		worldtransform.matWorld_ *= matRotY;
	}
	// matWorld_に移動量を掛け算
	worldtransform.matWorld_ *= matTrans;

	//行列の転送
	worldtransform.TransferMatrix();
}

#pragma endregion

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

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

	//スケーリングを設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	//回転角を設定
	worldTransform_.rotation_ = {PI / 4.0f, PI / 4.0f, 0.0f};

	//平行移動を設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};

	AffineTransformationFunction(worldTransform_);
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();
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

	// 3dモデル描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

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
