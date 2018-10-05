#pragma once

#include "Vec2.h"
#include "TileMap.h"
#include "Collideable.h"
#include "Graphics.h"

// Don't initialize one of these.
class EnemyBase
{
public:
	EnemyBase() = delete;
	EnemyBase( const Vec2& pos,const TileMap& map,
		const Collideable& coll ) = delete;

	// Children have to define these if they add new members (they will).
	EnemyBase( const EnemyBase& other );
	EnemyBase& operator=( const EnemyBase& other );
	EnemyBase( const EnemyBase&& other ) = delete;
	EnemyBase& operator=( const EnemyBase&& other ) = delete;
	
	// Children will implement these pls ty.
	virtual void Update( const Vec2& playerPos,float dt ) = 0;
	virtual void Draw( Graphics& gfx ) const = 0;

	virtual void Attack( const Vec2& whereFrom );

	const Vec2& GetPos() const;
	const Rect& GetRect() const;
	bool IsDead() const;
protected:
	EnemyBase( const Vec2& pos,const Vei2& size,
		const TileMap& map,const Collideable& coll,
		int hp );
protected:
	Vec2 pos;
	Rect hitbox;
	const Collideable& coll;
	const TileMap& map;
private:
	int hp;
	// YO!  If you add any new members, make sure to update copy assignment.
};