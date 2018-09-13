#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "TileMap.h"
#include "Collideable.h"
#include <vector>
#include "Bullet.h"
#include "Timer.h"

class Enemy
{
public:
	Enemy() = delete;
	Enemy( const Vec2& pos,const TileMap& map,
		const Collideable& coll,std::vector<Bullet>& bullets );

	void Update( const TileMap& map,
		const Vec2& playerPos,float dt );
	void Draw( Graphics& gfx ) const;
private:
	void UpdateTarget( const TileMap& map );
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 32,32 };
	Vec2 pos;
	Vei2 target;
	Vec2 vel;
	Rect hitbox;
	bool didCollide = false;
	static constexpr float speed = 90.0f;
	static constexpr float range = 15.0f;
	const Collideable& coll;
	std::vector<Bullet>& myBullets; // make sure this is a ref lol
	Timer shotTimer;
	static constexpr float minShotTime = 1.4f;
	static constexpr float maxShotTime = 2.1f;
};