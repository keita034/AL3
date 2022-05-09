#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	Vector3 vertex[8] = {
	  {0.0f,  0.0f,  0.0f },
	  {10.0f, 0.0f,  0.0f },
	  {10.0f, 0.0f,  10.0f},
	  {0.0f,  0.0f,  10.0f},
	  {0.0f,  10.0f, 0.0f },
	  {10.0f, 10.0f, 0.0f },
	  {10.0f, 10.0f, 10.0f},
	  {0.0f,  10.0f, 10.0f}
	};

	int edgeList[12][2] = {
	  {0, 1},
	  {1, 2},
	  {2, 3},
	  {3, 0},
	  {0, 4},
	  {1, 5},
	  {2, 6},
	  {3, 7},
	  {4, 5},
	  {5, 6},
	  {6, 7},
	  {7, 4},
	};

	//平行移動座標
	Vector3 translationVertex[8] = {
	  {0.0f,  0.0f,  0.0f },
	  {10.0f, 0.0f,  0.0f },
	  {10.0f, 0.0f,  10.0f},
	  {0.0f,  0.0f,  10.0f},
	  {0.0f,  10.0f, 0.0f },
	  {10.0f, 10.0f, 0.0f },
	  {10.0f, 10.0f, 10.0f},
	  {0.0f,  10.0f, 10.0f}
	};

	//拡大縮小座標
	Vector3 scaleVertex[8] = {
	  {0.0f,  0.0f,  0.0f },
	  {10.0f, 0.0f,  0.0f },
	  {10.0f, 0.0f,  10.0f},
	  {0.0f,  0.0f,  10.0f},
	  {0.0f,  10.0f, 0.0f },
	  {10.0f, 10.0f, 0.0f },
	  {10.0f, 10.0f, 10.0f},
	  {0.0f,  10.0f, 10.0f}
	};

	//回転座標
	Vector3 xrotateVertex[8] = {
	  {0.0f,  0.0f,  0.0f },
	  {10.0f, 0.0f,  0.0f },
	  {10.0f, 0.0f,  10.0f},
	  {0.0f,  0.0f,  10.0f},
	  {0.0f,  10.0f, 0.0f },
	  {10.0f, 10.0f, 0.0f },
	  {10.0f, 10.0f, 10.0f},
	  {0.0f,  10.0f, 10.0f}
	};

	Vector3 yrotateVertex[8] = {
	  {0.0f,  0.0f,  0.0f },
	  {10.0f, 0.0f,  0.0f },
	  {10.0f, 0.0f,  10.0f},
	  {0.0f,  0.0f,  10.0f},
	  {0.0f,  10.0f, 0.0f },
	  {10.0f, 10.0f, 0.0f },
	  {10.0f, 10.0f, 10.0f},
	  {0.0f,  10.0f, 10.0f}
	};

	Vector3 zrotateVertex[8] = {
	  {0.0f,  0.0f,  0.0f },
	  {10.0f, 0.0f,  0.0f },
	  {10.0f, 0.0f,  10.0f},
	  {0.0f,  0.0f,  10.0f},
	  {0.0f,  10.0f, 0.0f },
	  {10.0f, 10.0f, 0.0f },
	  {10.0f, 10.0f, 10.0f},
	  {0.0f,  10.0f, 10.0f}
	};

	uint32_t textureHandle_ = 0;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

};

