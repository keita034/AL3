#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"

#include <cassert>
#include <random>

#include "Player.h"

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
	enemyTextureHandle_ = TextureManager::Load("enemy.jpg");

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

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	//敵キャラの初期化
	enemy_->Initialize(model_, enemyTextureHandle_, Vector3(2.0f, 2.0f, 200.0f));
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());
}

void GameScene::Update() {
	//デバッグ時のみ有効
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_B)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif // !_DEBUG
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}

	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	if (enemy_) {
		enemy_->Update();
	}

	CheckAllCollisions();
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

	//自キャラの表示
	player_->Draw(viewProjection_);

	//敵キャラの表示
	if (enemy_) {
		enemy_->Draw(viewProjection_);
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

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullet();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullet();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = MyMath::Vector3Length(MyMath::Vector3Sub(posA, posB));

		//自キャラと敵弾の半径
		float radius = player_->GetRadius() + bullet->GetRadius();

		//自キャラと敵弾の交差判定
		if (distance <= radius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵のキャラの座標
	posA = enemy_->GetWorldPosition();

	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distance = MyMath::Vector3Length(MyMath::Vector3Sub(posA, posB));

		//敵キャラと自弾の半径
		float radius = enemy_->GetRadius() + bullet->GetRadius();

		//敵キャラと自弾の交差判定
		if (distance <= radius) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	//自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bulletB : enemyBullets) {

			//自弾の座標
			posB = bulletA->GetWorldPosition();
			//敵弾の座標
			posA = bulletB->GetWorldPosition();

			//座標AとBの距離を求める
			float distance = MyMath::Vector3Length(MyMath::Vector3Sub(posA, posB));

			//自弾と敵弾の半径
			float radius = bulletB->GetRadius() + bulletA->GetRadius();

			//自弾と敵弾の交差判定
			if (distance <= radius) {
				bulletB->OnCollision();
				bulletA->OnCollision();
			}
		}
	}
#pragma endregion
}
