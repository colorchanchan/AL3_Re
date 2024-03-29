#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "RailCamera.h"
#include "Skydome.h"

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

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void CheckAllcollsions();

	bool Changeflag;

	bool Gameover;

	bool Gameclear;

	int timer;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t titleHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
	uint32_t textureHandle2_ = 0;
	uint32_t titleHandle2_ = 0;
	Sprite* titleSprite2_ = nullptr;
	uint32_t gameOverHandle_ = 0;
	Sprite* gameOverSprite_ = nullptr;
	uint32_t gameClearHandle_ = 0;
	Sprite* gameClearSprite_ = nullptr;
	// 3Dモデルデータ
	Model* model_ = nullptr;
	Model* skyDomeModel_ = nullptr;
	// ワールドトランスフォーム
	// WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;
	// bool デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	std::list<Enemy*> enemies_;

	// 敵
	// Enemy* enemy_ = nullptr;

	// CollisionManager* collisionManager_;

	SkyDome* skyDome_;

	RailCamera* railCamera_;

	std::list<EnemyBullet*> enemyBullets_;
	std::stringstream enemyPopCommands;

	bool enemyPopWait;
	int32_t enemyPopWaitTimer;

private:
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	void EnemySpawn(Vector3 position, Vector3 velocity);
};
