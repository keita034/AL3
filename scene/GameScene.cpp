#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

const int WindowWidth = 1280; // 横幅
const int WindowHeight = 720; // 縦幅

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete ScopeSprite;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");
	ScopeTextureHandle = TextureManager::Load("reticle.png");

	//スプライトの生成
	ScopeSprite = Sprite::Create(ScopeTextureHandle, {0, 0});

	//スプライトの位置調整
	XMFLOAT2 ScopeSize = ScopeSprite->GetSize();
	XMFLOAT2 ScopePosition = {
	  WindowWidth / 2 - (ScopeSize.x / 2), WindowHeight / 2 - (ScopeSize.y / 2)};
	ScopeSprite->SetPosition(ScopePosition);

	random_device seedGen;

	mt19937_64 engine(seedGen());

	uniform_real_distribution<float> rotDist(0.0f, XM_2PI);

	uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		//スケーリングを設定
		worldTransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		//回転角を設定
		worldTransform_[i].rotation_ = {rotDist(engine), rotDist(engine), rotDist(engine)};
		//平行移動を設定
		worldTransform_[i].translation_ = {posDist(engine), posDist(engine), posDist(engine)};

		//ワールドトランスフォーム初期化
		worldTransform_[i].Initialize();
	}

	viewProjection_.fovAngleY = MaxfovAngleY;

	//ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

#pragma region 連続移動処理

#pragma endregion

	//視点の移動速さ
	float kEyeSpeed = 0.5f;

	//スコープ切り替え
	if (input_->PushKey(DIK_SPACE)) {
		if (viewProjection_.fovAngleY != MinfovAngleY) {
			viewProjection_.fovAngleY -= 0.05f;

			if (viewProjection_.fovAngleY < MinfovAngleY) {
				viewProjection_.fovAngleY = MinfovAngleY;
			}
		}

		kEyeSpeed = 0.3f;

	} else {
		if (viewProjection_.fovAngleY != MaxfovAngleY) {
			viewProjection_.fovAngleY += 0.05f;

			if (viewProjection_.fovAngleY > MaxfovAngleY) {
				viewProjection_.fovAngleY = MaxfovAngleY;
			}
		}
	}

	//視点の移動ベクトル
	XMFLOAT3 Eyemove = {0, 0, 0};

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_UP)) {
		Eyemove = {0, kEyeSpeed, 0};

	} else if (input_->PushKey(DIK_DOWN)) {
		Eyemove = {0, -kEyeSpeed, 0};
	}

	if (input_->PushKey(DIK_LEFT)) {
		Eyemove = {-kEyeSpeed, 0, 0};

	} else if (input_->PushKey(DIK_RIGHT)) {
		Eyemove = {kEyeSpeed, 0, 0};
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
	if (input_->PushKey(DIK_SPACE)) {
		ScopeSprite->Draw();
	}

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
