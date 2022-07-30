#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"

#include <cassert>
#include <fstream>
#include <random>

#include "Player.h"

GameScene::GameScene() {}

GameScene::~GameScene() { delete debugCamera_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("images/mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("images/enemy.jpg");
	//レティクルのテクスチャ
	TextureManager::Load("images/reticle.png");
	// 3Dモデル生成
	model_.reset(Model::Create());
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

	//天球の生成
	modelSkydome_ = std::make_unique<Skydome>();
	//天球の初期化
	modelSkydome_->Initialize();

	//レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f));

	//自キャラの生成
	player_ = std::make_shared<Player>();
	//自キャラの初期化
	player_->Initialize(
	  model_, textureHandle_, railCamera_->GetWorldTransformPtr(), Vector3(0.0f, 0.0f, 30.0f));

	LoadEnemyPopData("prams/EnemyScript.csv");
}

void GameScene::Update() {

	//レールカメラの更新
	Vector3 move(0.0f, 0.0f, -0.00f);
	Vector3 rot(0.0f, 0.0000f, 0.0f);
	railCamera_->Update(move, rot);
	viewProjection_ = railCamera_->GetViewProjection();

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
	player_->Update(viewProjection_);

	//敵発生コマンド
	UpdateEnemyPopCommands();

	//デスフラグの立った弾を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });

	//敵キャラの更新
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Update();
	}

	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
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

	//天球の表示
	modelSkydome_->Draw(viewProjection_);

	//自キャラの表示
	player_->Draw(viewProjection_);

	//敵キャラの表示
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	//敵キャラの弾表示
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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

	player_->DrawUI();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullet();

	//コライダー
	std::list<Collider*> colliders_;

	//コライダーをリストに登録
	//自キャラ
	colliders_.push_back(player_.get());

	//敵キャラのすべて
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		colliders_.push_back(enemy.get());
	}

	//敵弾全てについて
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		colliders_.push_back(bullet.get());
	}
	//自弾全てについて
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		colliders_.push_back(bullet.get());
	}

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	//衝突フィルタリング
	if (
	  colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask() ||
	  colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) {

	} else {
		return;
	}

	//判定対象AとBの座標
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	//座標AとBの距離を求める
	float distance = MyMath::Vector3Length(MyMath::Vector3Sub(posA, posB));

	//自弾と敵弾の半径
	float radius = colliderA->GetRadius() + colliderB->GetRadius();

	//球と球の交差判定
	if (distance <= radius) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData(const char* filepath) {
	//ファイルを開く
	std::ifstream file;
	file.open(filepath);
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (isStand_) {
		StandTime_--;
		if (StandTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}

		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (std::getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		//"//"から始まる行コメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす　
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			//敵キャラの生成
			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			//敵キャラの初期化
			newEnemy->Initialize(model_, enemyTextureHandle_, Vector3(x, y, z));
			//敵キャラにアドレスを渡す
			newEnemy->SetPlayer(player_);
			newEnemy->SetGameScene(this);

			//リストに登録する
			enemys_.push_back(std::move(newEnemy));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			StandTime_ = waitTime;

			//ループを抜ける
			break;
		}
	}
}
