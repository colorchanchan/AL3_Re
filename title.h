#pragma once
#include "Sprite.h"

class Title {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Sprite* sprite, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// スプライト
	Sprite* sprite_;
};