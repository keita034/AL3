#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>
#include<cmath>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld; // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0, 0, 0};
	// ローカル座標
	Vector3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	//スケーリング行列生成
	Matrix4 ScalingForm();

	// X軸回転行列を生成
	Matrix4 RotationXForm();

	// Y軸回転行列を生成
	Matrix4 RotationYForm();

	// Z軸回転行列を生成
	Matrix4 RotationZForm();

	// 回転行列を生成
	Matrix4 RotationForm();

	//平行移動行列を生成
	Matrix4 TransferForm();

	//ワールド行列を生成
	Matrix4 WorldForm();

	//アフィン変換
	void AffineTransformation();

	//親子関係更新
	void ParenChildUpdate();
};
