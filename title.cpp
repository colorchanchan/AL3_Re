#include "title.h"
#include <assert.h>

void Title::Initialize(Sprite* sprite, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(sprite);

	// 引数として受け取ったデータをメンバ変数に記録する
	sprite_ = sprite;
	textureHandle_ = textureHandle;
}

void Title::Update() {}

void Title::Draw() { sprite_->Draw(); }
