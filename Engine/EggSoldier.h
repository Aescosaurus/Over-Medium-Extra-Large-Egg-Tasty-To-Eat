#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include "Timer.h"
#include <vector>
#include "Anim.h"
#include "Codex.h"

class EggSoldier
	:
	public EnemyBase
{
public:
	EggSoldier() = delete;
	EggSoldier( const Vec2& pos,const TileMap& map,
		const Collideable& coll,std::vector<Bullet>& bullets );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;

	EggSoldier( const EggSoldier& other );
	EggSoldier& operator=( const EggSoldier& other );

	void Attack( const Vec2& whereFrom ) override;
private:
	void UpdateTarget();

	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 64,64 };
	Vei2 target;
	Vec2 vel;
	bool didCollide = false;
	static constexpr float speed = 90.0f;
	static constexpr float range = 15.0f;
	std::vector<Bullet>& myBullets; // make sure this is a ref lol
	Timer shotTimer;
	static constexpr float minShotTime = 1.4f;
	static constexpr float maxShotTime = 2.1f;
	static constexpr float bulletSpeed = 70.0f;
	const Surface* const spriteSheet = Codex<Surface>::RetrieveSurf( "Images/EggEnemyAnim.bmp",{ 160 * 4,32 * 4 } );
	Anim legsRunning;
	std::vector<Anim> bodyCracking;
	int bodyBreakIndex = 0;
	// YO!  If you add any new members, make sure to update copy assignment.
};