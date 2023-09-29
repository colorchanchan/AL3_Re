#pragma once

#include "Model.h"
#include <WorldTransform.h>
#include "CreatedMath.h"
#include "EnemyBullet.h"

class GameScene;

class Player;

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Move(Vector3 velocity);
	void Fire();
	
	void OnCollision();

	Vector3 GetPosition() { return worldTransform_.translation_; }
	Vector3 GetSpeed() { return velocity_; }
	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }

	static const int kFireInterval = 60;
	int32_t fireTimer = 0;
	

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	bool IsDead() const { return isDead_; }

	enum class Phase {
		Approach,
		Leave,
	};

		float GetRadius() { return radius_; }

private:
	    float radius_ = 0.5f;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	Phase phase_ = Phase::Approach;
	
	bool isDead_ = false;

	//std::list<EnemyBullet*> bullets_;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;
};

