#pragma once
#include "TileMap.h"
#include "Player.h"
#include "Collideable.h"
#include <vector>
#include "Bullet.h"
#include <string>
#include "Stairs.h"
#include "KeyWall.h"
#include "Key.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SpikeWall.h"
#include "DeathBall.h"
#include <memory>
#include "EggSoldier.h"
#include "Eggsploder.h"

class Campaign
{
public:
	Campaign( Graphics& gfx );

	Campaign( const Campaign& other ) = delete;
	Campaign( const Campaign&& other ) = delete;
	Campaign& operator=( const Campaign& other ) = delete;
	Campaign& operator=( const Campaign&& other ) = delete;

	void UpdateAll( const Keyboard& kbd,
		const Mouse& mouse );
	void DrawAll();
private:
	void ChangeLevel( const std::string& nextLevel );
private:
	Graphics& gfx;
	std::string curLevel; // this has to precede any var that uses it.
	Collideable coll;
	TileMap tiles;
	Player guy;
	Stairs stairwell;
	// std::vector<Enemy> enemies;
	std::vector<std::unique_ptr<EnemyBase>> eggs;
	std::vector<Bullet> bullets;
	std::vector<Bullet> enemyBullets;
	std::vector<KeyWall> keyWalls;
	std::vector<SpikeWall> spikeWalls;
	std::vector<DeathBall> deathBalls;
	Key theKey = Key{ { -50,-50 } };
	bool gotKey = false;

#if !NDEBUG
	bool canChangeLvl = false;
#endif
};