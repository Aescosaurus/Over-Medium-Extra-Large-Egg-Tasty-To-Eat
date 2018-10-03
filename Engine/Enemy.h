#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "TileMap.h"
#include "Collideable.h"
#include <vector>
#include "Bullet.h"
#include "Timer.h"
#include "Anim.h"
#include "Codex.h"

class Enemy
{
public:
	// Enemy() = delete;
	Enemy( const Vec2& pos,const TileMap& map,
		const Collideable& coll,std::vector<Bullet>& bullets );

	Enemy( const Enemy& other );
	Enemy& operator=( const Enemy& other );

	void Update( const TileMap& map,
		const Vec2& playerPos,float dt );
	void Draw( Graphics& gfx ) const;
	
	void Attack();

	const Vec2& GetPos() const;
	const Rect& GetRect() const;
	bool IsDead() const;
private:
	void UpdateTarget( const TileMap& map );
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	Vec2 pos;
	Vei2 target;
	Vec2 vel;
	Rect hitbox;
	bool didCollide = false;
	static constexpr float speed = 90.0f;
	static constexpr float range = 15.0f;
	const Collideable& coll;
	std::vector<Bullet>& myBullets; // make sure this is a ref lol
	const TileMap& tilemap;
	Timer shotTimer;
	static constexpr float minShotTime = 1.4f;
	static constexpr float maxShotTime = 2.1f;
	static constexpr float bulletSpeed = 70.0f;
	int hp = 5;
	// const AescPtr<Surface> spriteSheet = Codex
	// 	::GetSurfPtr( "Images/EggEnemyAnim.bmp",{ 160 * 4,32 * 4 } );
	const Surface* const spriteSheet = Codex<Surface>::RetrieveSurf( "Images/EggEnemyAnim.bmp",{ 160 * 4,32 * 4 } );
	Anim legsRunning;
	std::vector<Anim> bodyCracking;
	int bodyBreakIndex = 0;
	// YO!  If you add any new members, make sure to update copy assignment.
};