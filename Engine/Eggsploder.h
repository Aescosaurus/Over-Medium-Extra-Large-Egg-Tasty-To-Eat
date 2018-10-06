#pragma once

#include "EnemyBase.h"
#include "Codex.h"
#include "Anim.h"
#include "Timer.h"

class Eggsploder
	:
	public EnemyBase
{
public:
	Eggsploder() = delete;
	Eggsploder( const Vec2& pos,const TileMap& map,
		const Collideable& coll );

	Eggsploder( const Eggsploder& other );
	Eggsploder& operator=( const Eggsploder& other );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	void Attack( const Vec2& whereFrom ) override;
private:
	static constexpr Vei2 size = { 32,32 };
	static constexpr int myHP = 4;
	const Surface* const sprSheet = Codex<Surface>::RetrieveSurf( "Images/EggsploderAnim.bmp",{ 4,4 } );
	Anim legsRunning;
	Anim bodyBreaking;
	int bodyCrackIndex = 0;
	Timer retarget;
	static constexpr float retargetTime = 0.4f;
	Vec2 vel;
	static constexpr float speed = 124.1f;
	static constexpr float knockbackPower = 24.5f;
};