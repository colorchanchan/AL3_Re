#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	// delete collisionManager_;

	delete skyDome_;
	delete skyDomeModel_;
	delete railCamera_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("player.png");
	titleHandle_ = TextureManager::Load("title.png");
	titleSprite_ = Sprite::Create(titleHandle_, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f,0.5f});
	// 3Dモデルの生成

	TextureManager::Load("Reticle.png");

	model_ = Model::Create();
	skyDomeModel_ = Model::CreateFromOBJ("skydome", true);
	// ワールドトランスフォーム
	// worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, {0.0f, 0.0f, 0.0f});
	Vector3 playerPosition(0, 0, 30);
	//	EnemySpawn({14.0f, 0.0f, 40.0f}, {0.0f, 0.0f, -0.5f});

	skyDome_ = new SkyDome();
	skyDome_->Initialize(skyDomeModel_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	LoadEnemyPopData();

	Changeflag = false;
}

void GameScene::Update() {
	if (Changeflag == true) {

		player_->Update(viewProjection_);
		UpdateEnemyPopCommands();

		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
		for (EnemyBullet* bullet : enemyBullets_) {

			bullet->Update();
		}

		skyDome_->Update();

		CheckAllcollsions();
	} 
	else {
		XINPUT_STATE joyState;
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}    
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			Changeflag = true;
		}
	}
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
	skyDome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {

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
	if (Changeflag == false) {
		titleSprite_->Draw();
	} else {
		player_->DrawUI();
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion


}
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

void GameScene::CheckAllcollsions() {
	Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region プレイヤーと敵の弾

	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();

		float dx = posA.x - posB.x;
		float dy = posA.y - posB.y;
		float dz = posA.z - posB.z;
		float distance = dx * dx + dy * dy + dz * dz;
		float r = (player_->GetRadius() + bullet->GetRadius());
		if (distance <= r * r) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 敵と自機の弾

	for (Enemy* enemy : enemies_) {
		for (PlayerBullet* playerbullet : playerBullets) {
			posA = enemy->GetPosition();
			posB = playerbullet->GetWorldPosition();

			float dx = posA.x - posB.x;
			float dy = posA.y - posB.y;
			float dz = posA.z - posB.z;
			float distance = dx * dx + dy * dy + dz * dz;

			if (distance <= (enemy->GetRadius() + playerbullet->GetRadius()) *
			                    (enemy->GetRadius() + playerbullet->GetRadius())) {
				enemy->OnCollision();
				playerbullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	if (enemyPopWait == true) {
		enemyPopWaitTimer--;
		if (enemyPopWaitTimer <= 0) {
			enemyPopWait = false;
		}
		return;
	}
	std::string line;

	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');
		if (word.find("//") == 0) {

			continue;
		}
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			EnemySpawn({x, y, z}, {0.0f, 0.0f, -0.5f});
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());
			enemyPopWait = true;
			enemyPopWaitTimer = waitTime;

			break;
		}
	}
}

void GameScene::EnemySpawn(Vector3 position, Vector3 velocity) {
	Enemy* enemy = new Enemy();
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemy->Initialize(model_, position, velocity);

	enemies_.push_back(enemy);
}