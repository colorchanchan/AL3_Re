#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Player;

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void OnCollision();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	float GetRadius() { return radius_; }

private:
	float radius_ = 0.5f;
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;

	Vector3 velocity_;

	static const int32_t kLifeTime = 120;
	int32_t deathTimer = kLifeTime;

	bool isDead_ = false;

	Player* player_;
};