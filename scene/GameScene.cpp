#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

enum Coordinate { X, Y, Z };

#define PI 3.141592653589

//平行移動
void Translation(Vector3 position[], Vector3 Translation) {

	float w;

	//平行移動
	for (int i = 0; i < 8; i++) {
		
		position[i].x =
		  1.0f * position[i].x + 0.0f * position[i].y + 0.0f * position[i].z + 1.0f * Translation.x;
		
		position[i].y =
		  0.0f * position[i].x + 1.0f * position[i].y + 0.0f * position[i].z + 1.0f * Translation.y;
		
		position[i].z =
		  0.0f * position[i].x + 0.0f * position[i].y + 1.0f * position[i].z + 1.0f * Translation.z;

		w = 0.0f * position[i].x + 0.0f * position[i].y + 0.0f * position[i].z + 1.0f * 1.0f;
	}
}

//拡大縮小
void Scale(Vector3 position[], Vector3 scale) {

	float w;

	//原点移動量
	Vector3 OriginPosition = {position[0].x, position[0].y, position[0].z};

	//計算用
	float TmpPosition[3] = {};

	//原点へ
	Translation(position, -OriginPosition);

	for (int i = 0; i < 8; i++) {

		//座標を保存
		TmpPosition[X] = position[i].x;
		TmpPosition[Y] = position[i].y;
		TmpPosition[Z] = position[i].z;

		////拡大縮小
		position[i].x =
		  scale.x * TmpPosition[X] + 0.0f * TmpPosition[Y] + 0.0f * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].y =
		  0.0f * TmpPosition[X] + scale.y * TmpPosition[Y] + 0.0f * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].z =
		  0.0f * TmpPosition[X] + 0.0f * TmpPosition[Y] + scale.z * TmpPosition[Z] + 0.0f * 0.0f;

		w = 0.0f * position[i].x + 0.0f * position[i].y + 0.0f * position[i].z + 1.0f * 1.0f;
	}

	//元に戻す
	Translation(position, OriginPosition);
}

//X軸周りの回転
void XRotate(Vector3 position[], float angle)
{
	float w = 0.0f;

	//原点移動量
	Vector3 OriginPosition = { position[0].x, position[0].y, position[0].z };

	//計算用
	float TmpPosition[3] = {};

	Translation(position, -OriginPosition);

	//縮小拡大
	for (int i = 0; i < 8; i++)
	{

		//座標を保存
		TmpPosition[X] = position[i].x;
		TmpPosition[Y] = position[i].y;
		TmpPosition[Z] = position[i].z;

		//回転
		position[i].x =
			1.0f * TmpPosition[X] + 0.0f * TmpPosition[Y] + 0.0f * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].y = 0.0f * TmpPosition[X] + cos(angle) * TmpPosition[Y] +
			-sinf(angle) * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].z = 0.0f * TmpPosition[X] + sinf(angle) * TmpPosition[Y] +
			cosf(angle) * TmpPosition[Z] + 0.0f * 0.0f;

		w = 0.0f * position[i].x + 0.0f * position[i].y + 0.0f * position[i].z + 1.0f * 1.0f;
	}

	//元に戻す
	Translation(position, OriginPosition);
}

//Y軸周りの回転
void YRotate(Vector3 position[], float angle) {
	float w = 0.0f;

	//原点移動量
	Vector3 OriginPosition = {position[0].x, position[0].y, position[0].z};

	//計算用
	float TmpPosition[3] = {};

	//原点へ
	Translation(position, -OriginPosition);

	for (int i = 0; i < 8; i++) {

		//座標を保存
		TmpPosition[X] = position[i].x;
		TmpPosition[Y] = position[i].y;
		TmpPosition[Z] = position[i].z;

		//回転
		position[i].x = cosf(angle) * TmpPosition[X] + 0.0f * TmpPosition[Y] +
		                sinf(angle) * TmpPosition[Z] + 0.0f * 0.0f;
		position[i].y =
		  0.0f * TmpPosition[X] + 1.0f * TmpPosition[Y] + 0.0f * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].z = -sinf(angle) * TmpPosition[X] + 0.0f * TmpPosition[Y] +
		                cosf(angle) * TmpPosition[Z] + 0.0f * 0.0f;

		w = 0.0f * position[i].x + 0.0f * position[i].y + 0.0f * position[i].z + 0.0f * 1.0f;
	}

	//元に戻す
	Translation(position, OriginPosition);
}

//Z軸周りの回転
void ZRotate(Vector3 position[], float angle) {
	float w = 0.0f;

	//原点移動量
	Vector3 OriginPosition = {position[0].x, position[0].y, position[0].z};

	//計算用
	float TmpPosition[3] = {};

	//原点へ
	Translation(position, -OriginPosition);

	for (int i = 0; i < 8; i++) {

		//座標を保存
		TmpPosition[X] = position[i].x;
		TmpPosition[Y] = position[i].y;
		TmpPosition[Z] = position[i].z;

		//回転
		position[i].x = cosf(angle) * TmpPosition[X] + -sinf(angle) * TmpPosition[Y] +
		                0.0f * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].y = sinf(angle) * TmpPosition[X] + cosf(angle) * TmpPosition[Y] +
		                0.0f * TmpPosition[Z] + 0.0f * 0.0f;

		position[i].z =
		  0.0f * TmpPosition[X] + 0.0f * TmpPosition[Y] + 1.0f * TmpPosition[Z] + 0.0f * 0.0f;

		w = 0.0f * position[i].x + 0.0f * position[i].y + 0.0f * position[i].z + 1.0f * 1.0f;
	}

	//元に戻す
	Translation(position, OriginPosition);
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

	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	AxisIndicator::GetInstance()->SetVisible(true);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	
	//平行移動
	Vector3 Move = {20.0f, 20.0f, 20.0f};
	Translation(translationVertex, Move);

	//拡大縮小
	Vector3 scaleMove = { 2.0f, 2.0f, 2.0f };
	Scale(scaleVertex, scaleMove);

	//回転
	float angle = PI / 4.0f;
	XRotate(xrotateVertex, angle);
	YRotate(yrotateVertex, angle);
	ZRotate(zrotateVertex, angle);


}

void GameScene::Update() { debugCamera_->Update(); }

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

	for (int i = 0; i < 12; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vertex[edgeList[i][0]], vertex[edgeList[i][1]], Vector4(0xff, 0xff, 0xff, 0xff));
		
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			translationVertex[edgeList[i][0]], translationVertex[edgeList[i][1]], Vector4(0x00, 0xff, 0xff, 0xff));

		PrimitiveDrawer::GetInstance()->DrawLine3d(
			scaleVertex[edgeList[i][0]], scaleVertex[edgeList[i][1]], Vector4(0x00, 0x00, 0xff, 0xff));

		PrimitiveDrawer::GetInstance()->DrawLine3d(
			yrotateVertex[edgeList[i][0]], yrotateVertex[edgeList[i][1]], Vector4(0x00, 0x00, 0x00, 0xff));

		PrimitiveDrawer::GetInstance()->DrawLine3d(
			xrotateVertex[edgeList[i][0]], xrotateVertex[edgeList[i][1]], Vector4(0xff, 0x00, 0x00, 0xff));

		PrimitiveDrawer::GetInstance()->DrawLine3d(
			zrotateVertex[edgeList[i][0]], zrotateVertex[edgeList[i][1]], Vector4(0x00, 0xff, 0x00, 0xff));
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
